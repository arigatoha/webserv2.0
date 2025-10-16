#include "RequestHandler.hpp"

#include <sys/stat.h>
#include <ctime>
#include <cstdlib>
#include <limits.h>

/*ERROR TEMPLATE*/
// append static http version and error msg
// calculate time
// static webserv/version
// content-length size/len of the file
// content-type get filetype and charset from the file?
// connection close/redirect/?

std::string		RequestHandler::getHttpDate() {
	time_t now = time(NULL);

	struct tm *gmt_time = gmtime(&now);

	char buffer[128];
	// Thu, 09 Oct 2025 09:01:45 GMT
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt_time);

	return std::string(buffer);
}

std::string	RequestHandler::create_404_response(std::string &errorText) {
	std::string		response;

	response = "HTTP/1.1 404 not Found\r\n";
	response += getHttpDate() + "\r\n";
	response += "Webserv/ver 1.0\r\n"
}

std::string	RequestHandler::create_403_response(std::string &errorText) {
	return "403 Forbidden\n";
}

std::string	RequestHandler::create_500_response(std::string &errorText) {
	return "500 Internal Server Error\n";
}

std::string RequestHandler::create_200_response() {
	return "200 OK\n";
}


const std::string	RequestHandler::getDefaultError(ErrorCode status_code) {
	switch(status_code) {
		case NOTF_404:
		return "<!DOCTYPE html><html><body><h1>404 Not Found</h1></body></html>";
		case FORB_403:
		return "<!DOCTYPE html><html><body><h1>403 Forbidden</h1></body></html>";
		case SERV_500:
		return "<!DOCTYPE html><html><body><h1>500 Internal Server Error</h1></body></html>";
		default:
		return "<!DOCTYPE html><html><body><h1>Error</h1></body></html>";
	}
}

const Location	*RequestHandler::findBestLocationMatch(const Config &serv_cfg, const std::string &url) {
	const Location	*best_match = nullptr;
	size_t			longest_len = 0;
	
	const std::vector<Location>	&locations = serv_cfg.getLocations();
	for (size_t i = 0; i < locations.size(); ++i) {
		const std::string &loc_path = locations[i].getPath();
		if (url.find(loc_path, 0) != url.npos) {
			if (loc_path.length() > longest_len) {
				longest_len = loc_path.length();
				best_match = &locations[i];
			}
		}
	}
	return best_match;
}

bool RequestHandler::normalizePath(const std::string &input_path, std::string &resolved_path) {
	char actual_path[PATH_MAX];
	
	if (realpath(input_path.c_str(), actual_path) == NULL)
	return false;
	resolved_path = actual_path;
	return true;
}

std::string RequestHandler::handle(const Config &serv_cfg, HttpRequest &req) {
	resolveRequestToAction()
}

ResolvedAction	RequestHandler::resolveErrorAction(int error_code, const Config &serv_cfg) {
	std::string		error_url;

	if (serv_cfg.getErrorPage(error_code, error_url)) {
		const Location *location = findBestLocationMatch(serv_cfg, error_url);

		std::string root = location->getPath();
		std::string file_path = root + error_url;
		
		struct stat st;
		if (stat(file_path.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
			ResolvedAction	action;
			
			action.st = st;
			action.status_code = error_code;
			action.target_path = file_path;
			action.type = ACTION_SERVE_FILE;
			
			return action;
		}
	}
	ResolvedAction	default_action;

	default_action.status_code = error_code;
	default_action.type = ACTION_GENERATE_ERROR;
	
	return default_action;
}

ResolvedAction	RequestHandler::resolveRequestToAction(const Config &serv_cfg, HttpRequest &req) {
	std::string		filename;
	
	const Location *location = findBestLocationMatch(serv_cfg, req.getPath());
	if (location == NULL) {
		return resolveErrorAction(404, serv_cfg);
	}
	if (normalizePath(location->getPath() + req.getPath(), filename) == false) {
		return resolveErrorAction(404, serv_cfg);
	}
	return checkReqPath(filename, serv_cfg, location);
}

ResolvedAction RequestHandler::resolveFileAction(const std::string &path, const Config &cfg, struct stat *st) {
	ResolvedAction	action;

	action.st = *st;
	action.status_code = 200;
	action.type = ACTION_SERVE_FILE;
	action.target_path = path;

	return action;
}

bool	RequestHandler::findAccessibleIndex(ResolvedAction &action, const std::string &dir_path,
											const std::vector<std::string> &indexes) {
	for (size_t i = 0; i < indexes.size(); ++i) {
		std::string full_path = dir_path + "/" + indexes[i];
		
		struct stat st;

		if (stat(full_path.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
			action.st = st;
			action.status_code = 200;
			action.target_path = full_path;
			action.type = ACTION_SERVE_FILE;
			return true;
		}
	}
	return false;
}
ResolvedAction RequestHandler::resolveDirAction(const std::string &dir_path, const Config &cfg,
												struct stat *st, const Location *location) {
	ResolvedAction				action;
	std::vector<std::string>	indexes;

	if (location->getIndexes(indexes) && findAccessibleIndex(action, dir_path, indexes))
		return action;
	if (cfg.getIndexes(indexes) && findAccessibleIndex(action, dir_path, indexes))
		return action;
	std::string	autoindex;
	if (location->getDirective("autoindex", autoindex) && autoindex == "on") {
		action.st = *st;
		action.status_code = 200;
		action.target_path = dir_path;
		action.type = ACTION_AUTOINDEX;
		return action;
	}
	return resolveErrorAction(403, cfg);
}


ResolvedAction	RequestHandler::checkReqPath(const std::string &path, const Config &cfg, const Location *location) {
	struct stat st;

	if (stat(path.c_str(), &st) != 0) {
		switch(errno) {
			case ENOENT:
				return resolveErrorAction(404, cfg);
			case EACCES:
				return resolveErrorAction(403, cfg);
			default:
				return resolveErrorAction(500, cfg);
		}
	}
	if (S_ISDIR(st.st_mode)) {
		return resolveDirAction(path, cfg, &st, location);
	}
	else if (S_ISREG(st.st_mode))
		return resolveFileAction(path, cfg, &st);
	//Fallback
	return resolveErrorAction(403, cfg);
}

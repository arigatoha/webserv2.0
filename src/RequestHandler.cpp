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

void	RequestHandler::checkReqPath(const std::string &path, PathInfo &pathInfo) {
	if (stat(path.c_str(), &pathInfo.st) != 0) {
		switch(errno) {
			case ENOENT:
				pathInfo.error = NOTF_404;
				pathInfo.type = PATH_NOT_FOUND;
				return ;
			case EACCES:
				pathInfo.error = FORB_403;
				pathInfo.type = PATH_IS_OTHER;
				return ;
			default:
				pathInfo.error = SERV_500;
				pathInfo.type = PATH_IS_OTHER;
				return ;
		}
	}
	if (S_ISDIR(pathInfo.st.st_mode)) {
		pathInfo.error = FORB_403;
		pathInfo.type = PATH_IS_DIR;
		return ;
	}
	pathInfo.type = PATH_IS_FILE;
	pathInfo.error = SUCC_200;
	return ;
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

std::string	RequestHandler::resolvePath(const std::string &request, const Location *location, const Config &cfg) {
	std::string result;
	PathInfo	pathInfo;

	if (normalizePath(location->getPath() + request, result) == false) {
		pathInfo.error = NOTF_404;
		return "";
	}
	checkReqPath(result, pathInfo);
	if () {
		return "";
	}
	return result;
}

std::string RequestHandler::handle(const Config &serv_cfg, HttpRequest &req) {

}

std::string RequestHandler::handlePath(const Config &serv_cfg, HttpRequest &req) {
	std::string		errorUrl;
	std::string		filePath;
	ErrorCode		error;

	error = SUCC_200;
	const Location *location = findBestLocationMatch(serv_cfg, req.getPath());
	if (location == nullptr)
		error = NOTF_404;
	else {
		filePath = resolvePath(req.getPath(), location, serv_cfg);
	}
	if (error != SUCC_200) {
		if (serv_cfg.getErrorPage(error, errorUrl)) {
			req.setPath(errorUrl);
			handlePath(serv_cfg, req); // potentially infinite recursion TODO
		}
		else {
			return getDefaultError(error);
		}
	}

}

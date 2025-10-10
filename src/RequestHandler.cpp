#include "RequestHandler.hpp"

#include <sys/stat.h>
#include <ctime>

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

std::string	RequestHandler::create_404_response() {
	std::string		response;

	response = "HTTP/1.1 404 not Found\r\n";
	response += getHttpDate() + "\r\n";
	response += "Webserv/ver 1.0\r\n"
}

std::string	RequestHandler::create_403_response() {
	return "403 Forbidden\n";
}

std::string	RequestHandler::create_500_response() {
	return "500 Internal Server Error\n";
}

std::string RequestHandler::create_200_response() {
	return "200 OK\n";
}

RequestHandler::ErrorCode	RequestHandler::checkReqPath(const std::string &path) {
	struct stat         st;

	if (stat(path.c_str(), &st) != 0) {
		switch(errno) {
			case ENOENT:
				return create_404;
			case EACCES:
				return create_403;
			default:
				return create_500;
		}
	}
	if (S_ISDIR(st.st_mode)) {
		return create_403;
	}
	return create_200;
}

std::string RequestHandler::generate_response(const Config &serv_cfg, const HttpRequest &req) {
	ErrorCode		status_code;
	std::string		error_path;

	status_code = this->checkReqPath(req.path);
	if (status_code != create_200) {
		error_path = serv_cfg.getErrorPage(status_code);
		if (error_path == "") {
			
		}
	}
}

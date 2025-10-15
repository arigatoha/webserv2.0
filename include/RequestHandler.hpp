#pragma once

#include <string>
#include "httpRequest.hpp"
#include "Config.hpp"
#include "pathUtils.hpp"

class RequestHandler {
	public:
		RequestHandler();
		~RequestHandler();
		
		RequestHandler(const RequestHandler &other);
		RequestHandler &operator=(const RequestHandler &other);
		
		std::string		handle(const Config &serv_cfg, HttpRequest &req);
		
	private:

		const std::string	getDefaultError(ErrorCode status_code);
		std::string			handle_parsed_request(const HttpRequest &req, int client_fd);
		void				checkReqPath(const std::string &path, PathInfo &PathInfo);
		std::string			getHttpDate();
		const Location		*findBestLocationMatch(const Config &serv_cfg, const std::string &url);
		std::string			resolvePath(const std::string &request, const Location *location, const Config &cfg);
		bool				normalizePath(const std::string &input_path, std::string &resolved_path);
		std::string RequestHandler::handlePath(const Config &serv_cfg, HttpRequest &req);

		std::string		create_404_response(std::string &errorText);
		std::string		create_403_response(std::string &errorText);
		std::string		create_500_response(std::string &errorText);
		std::string		create_200_response();
};
#pragma once

#include <string>
#include "httpRequest.hpp"
#include "Config.hpp"

class RequestHandler {
	public:
		RequestHandler();
		~RequestHandler();
		
		RequestHandler(const RequestHandler &other);
		RequestHandler &operator=(const RequestHandler &other);
		
		std::string		generate_response(const Config &serv_cfg, const HttpRequest &req);
		
	private:

		std::string		handle_parsed_request(const HttpRequest &req, int client_fd);
		ErrorCode		checkReqPath(const std::string &path);
		std::string		getHttpDate();

		std::string		create_404_response();
		std::string		create_403_response();
		std::string		create_500_response();
		std::string		create_200_response();
};
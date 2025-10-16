#pragma once

#include <string>
#include "httpRequest.hpp"
#include "Config.hpp"
#include "ResolvedAction.hpp"

class RequestHandler {
	public:
		RequestHandler();
		~RequestHandler();
		
		RequestHandler(const RequestHandler &other);
		RequestHandler &operator=(const RequestHandler &other);
		
		std::string		handle(const Config &serv_cfg, HttpRequest &req);
		
	private:

		std::string			handle_parsed_request(const HttpRequest &req, int client_fd);
		std::string			getHttpDate();
		const Location		*findBestLocationMatch(const Config &serv_cfg, const std::string &url);
		bool				normalizePath(const std::string &input_path, std::string &resolved_path);
		std::string			handlePath(const Config &serv_cfg, HttpRequest &req);
		ResolvedAction		resolveRequestToAction(const Config &serv_cfg, HttpRequest &req);
		ResolvedAction		checkReqPath(const std::string &path, const Config &cfg, const Location *location);
		ResolvedAction		resolveErrorAction(int error_code, const Config &serv_cfg);
		ResolvedAction		resolveFileAction(const std::string &path, const Config &cfg, struct stat *st);
		ResolvedAction		resolveDirAction(const std::string &path, const Config &cfg, struct stat *st,
								const Location *location);
		bool				findAccessibleIndex(ResolvedAction &action, const std::string &dir_path,
								const std::vector<std::string> &indexes);


		std::string		create_404_response(std::string &errorText);
		std::string		create_403_response(std::string &errorText);
		std::string		create_500_response(std::string &errorText);
		std::string		create_200_response();
};
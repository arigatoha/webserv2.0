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
		
		void		handle(const Config &serv_cfg, const HttpRequest &req, int client_fd);
		
	private:

		std::string			getHttpDate();
		const Location		*findBestLocationMatch(const Config &serv_cfg, const std::string &url);
		bool				normalizePath(const std::string &input_path, std::string &resolved_path);
		std::string			handlePath(const Config &serv_cfg, HttpRequest &req);
		ResolvedAction		resolveRequestToAction(const Config &serv_cfg, const std::string &req_path);
		ResolvedAction		checkReqPath(const std::string &path, const Config &cfg, const Location *location);
		ResolvedAction		resolveErrorAction(int error_code, const Config &serv_cfg);
		ResolvedAction		resolveFileAction(const std::string &path, const Config &cfg, struct stat *st);
		ResolvedAction		resolveDirAction(const std::string &path, const Config &cfg, struct stat *st,
								const Location *location);
		bool				findAccessibleIndex(ResolvedAction &action, const std::string &dir_path,
								const std::vector<std::string> &indexes);
		void				sendString(int client_fd, const std::string &response);
		void				streamFileBody(int client_fd, const std::string &file_path);
								
		const std::string	getDefaultError(int status_code);
		std::string			genAutoindexAction(const ResolvedAction &action);
		std::string			createDirListHtml(const std::string &physical_path,
									const std::string &logic_path);

		void			sendDefaultError(int status_code, int client_fd);
		void			sendFile(const ResolvedAction &action, int client_fd);
		void			sendDir(const std::string &phys_path, int client_fd, const std::string &logic_path);

		const std::string		generic_error_response();
		const std::string		create_404_response();
		const std::string		create_403_response();
		const std::string		create_500_response();
		const std::string		createSuccResponseHeaders(long int contentLen);

};
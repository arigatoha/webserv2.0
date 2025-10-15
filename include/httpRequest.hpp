#pragma once

#include <string>
#include <map>

class HttpRequest {
	public:

		HttpRequest();
		~HttpRequest();

		HttpRequest(const HttpRequest &other);
		HttpRequest &operator=(const HttpRequest &other);

		const std::string	getPath() const;
		void				setPath(std::string &path);
	
	private:
		std::string							method;
		std::string							path;
		std::string							query_str;
		std::string							http_ver;
		std::string							body;
		std::map<std::string, std::string>	headers;
};

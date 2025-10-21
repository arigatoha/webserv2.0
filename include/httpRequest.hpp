#pragma once

#include <string>
#include <map>

class HttpRequest {
	public:

		HttpRequest();
		~HttpRequest();

		HttpRequest(const HttpRequest &other);
		HttpRequest &operator=(const HttpRequest &other);

		const std::string	&getPath() const;
		const std::string	&getMethod() const;
		const std::string	&getVersion() const;
		const std::string	&getQuery() const;
		const std::string	&getBody() const;
		const std::string	&getHeader(const std::string &key) const;

		void				setPath(const std::string &path);
		void				setMethod(const std::string &method);
		void				setVersion(const std::string &version);
		void				setQuery(const std::string &query);	
		void				setBody(const std::string &body);
		void				setHeaders(const std::string &Headers);

	private:
		std::string							_method;
		std::string							_path;
		std::string							_query_str;
		std::string							_http_ver;
		std::string							_body;
		std::map<std::string, std::string>	_headers;
};

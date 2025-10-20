#include "httpRequest.hpp"

const std::string	HttpRequest::getPath() const {
	return this->_path;
}
void				HttpRequest::setPath(const std::string &path) {
	this->_path = path;
}

const std::string	HttpRequest::getMethod() const { return this->_method;}
const std::string	HttpRequest::getVersion() const { return this->_http_ver;}
const std::string	HttpRequest::getQuery() const { return this->_query_str;}

void	HttpRequest::setMethod(const std::string &method) { this->_method = method;}
void	HttpRequest::setVersion(const std::string &version) { this->_http_ver = version;}
void	HttpRequest::setQuery(const std::string &query) { this->_query_str = query;}

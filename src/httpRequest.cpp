#include "httpRequest.hpp"

const std::string	HttpRequest::getPath() const {
	return this->path;
}
void				HttpRequest::setPath(std::string &_path) {
	this->path = _path;
}

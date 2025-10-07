#include "Location.hpp"
#include "StringUtils.hpp"
#include <iostream>

const std::string	&Location::getPath(void) const {
	return this->_path;
}

void				Location::setPath(const std::string &path) {
	this->_path = path;
}

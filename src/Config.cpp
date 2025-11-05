#include "Config.hpp"
#include "StringUtils.hpp"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

Config::Config() {
}

Config::~Config() {}

Config::Config(const Config &other) : AConfigBlock(other), error_pages(other.error_pages), _locations(other._locations) {}

Config &Config::operator=(const Config &other) {
	if (this != &other) {
		AConfigBlock::operator=(other);
		this->error_pages = other.error_pages;
		this->_locations = other._locations;
	}
	return *this;
}

bool    Config::checkIfDuplicate(const std::string &path) const{
    std::vector<Location>::const_iterator    it;

    for (it = _locations.begin(); it != _locations.end(); it++) {
        if (it->getPath() == path) {
            return true;
        }
    }
    return false;
}

const std::vector<Location>		&Config::getLocations() const {
	return this->_locations;
}

bool		Config::getErrorPage(int code, std::string &errorPage) const {
    std::map<int, std::string>::const_iterator	it;

    it = error_pages.find(code);
    if (it == error_pages.end())
        return false;
    errorPage = it->second;
    return true;
}

bool Config::getPort(const std::string &key, std::string &out_val) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = _directives.find(key);
	if (it == _directives.end())
		return false;
	out_val = it->second;
	return true;
}

Location					&Config::getNewLocation() {
    _locations.push_back(Location());
    return _locations.back();
}

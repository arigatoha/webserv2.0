#include "Config.hpp"
#include "StringUtils.hpp"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

#define ERROR_CODE 0
#define ERROR_PAGE 1

Config::Config() {
	_directives["listen"] = "8080";
	_directives["server_name"] = "localhost";
}

Config::~Config() {}

Config::Config(const Config &other) : ADirectives(other), error_pages(other.error_pages), _locations(other._locations) {}

Config &Config::operator=(const Config &other) {
	if (this != &other) {
		ADirectives::operator=(other);
		this->error_pages = other.error_pages;
		this->_locations = other._locations;
	}
	return *this;
}

void    Config::setError_page(const std::string &value) {
    std::vector<std::string>		tokens;
    long							error_code_long;
	char							*end;

    tokens = StringUtils::split(value, ' ');
    error_code_long = strtol(tokens.at(ERROR_CODE).c_str(), &end, 10);
    this->error_pages[static_cast<int>(error_code_long)] = tokens.at(ERROR_PAGE);
}

bool    Config::addLocation(const Location &loc) {
    std::vector<Location>::iterator    it;

    for (it = _locations.begin(); it != _locations.end(); it++) {
        if (it->getPath() == loc.getPath()) {
            return false;
        }
    }
    _locations.push_back(loc);
    return true;
}

bool			Config::getErrorPage(ErrorCode error, std::string &path_out) const {
	std::map<int, std::string>::const_iterator it = this->error_pages.find(error);

	if (it != error_pages.end()) {
		path_out = it->second;
		return true;
	}
	return false;
}

const std::vector<Location>		&Config::getLocations() const {
	return this->_locations;
}

#include "Config.hpp"
#include "StringUtils.hpp"
#include <iostream>

#define ERROR_CODE 0
#define ERROR_PAGE 1

void    Config::setError_page(const std::string &value) {
    std::vector<std::string>    tokens;
    int                         error_code;

    tokens = StringUtils::split(value, ' ');
    error_code = std::atoi(tokens.at(ERROR_CODE).c_str());
    this->error_pages[error_code] = tokens.at(ERROR_PAGE);
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

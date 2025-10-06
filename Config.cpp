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

bool    Config::setDirective(const std::string &key, const std::string &value) {
    if (key == "error_page") {
        setError_page(value);
        return true;
    }
    if (_simple_directives.find(key) != _simple_directives.end()) {
		std::cerr << "Error: Duplicate directive '" << key << "'." << std::endl;
		return false;
    }
	_simple_directives[key] = value;
	return true;
}

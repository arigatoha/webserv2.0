#include "AConfigBlock.hpp"
#include "StringUtils.hpp"
#include <iostream>

bool AConfigBlock::getDirective(const std::string &key, std::string &out_val) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = _directives.find(key);
	if (it == _directives.end())
		return false;
	out_val = it->second;
	return true;
}

void AConfigBlock::setDirective(const std::string &key, const std::string &value) {
	if (_directives.find(key) == _directives.end())
		return ;
	_directives[key] = value;
}

bool AConfigBlock::getMultiDirective(const std::string &key, std::vector<std::string> &out_val) const {
	std::map<std::string, std::vector<std::string> >::const_iterator	it;

	it = _multiDirectives.find(key);
	if (it == _multiDirectives.end())
		return false;
	out_val = it->second;
	return true;
}

bool AConfigBlock::isAutoindexOn() const {
	std::string Boolean;

	getDirective("autoindex", Boolean);
	return (Boolean == "on") ? true : false; 
}

bool AConfigBlock::getIndexes(std::vector<std::string> &out_val) const {
	return getMultiDirective("index", out_val);
}

void AConfigBlock::setMultiDirective(const std::string &key, const std::vector<std::string> &value) {
	if (_multiDirectives.find(key) == _multiDirectives.end())
		return ;
	_multiDirectives[key] = value;
}

void		AConfigBlock::setErrorPage(const std::string &error_code, const std::string &file) {
	if (_error_pages.find(error_code) == _error_pages.end())
		return ;
	_error_pages[error_code] = file;
}

bool		AConfigBlock::getErrorPage(int error_code, std::string &out_val) const {
	std::map<std::string, std::string>::const_iterator	it;

	std::string error_code_str = StringUtils::myItoa(error_code);
	it = _error_pages.find(error_code_str);
	if (it == _error_pages.end())
		return false;
	out_val = it->second;
	return true;

}

void		AConfigBlock::addLimitExceptRules(const std::string &key, const std::string &value) {
	if (key == "method")
		this->_rules._methods.push_back(value);
	else if (key == "allow")
		this->_rules._allow.push_back(value);
	else if (key == "deny")
		this->_rules._deny.push_back(value);
	else
		std::cerr << "WARNING.Unknown rule in limit_except. Continuing...\n";
	return ;
}

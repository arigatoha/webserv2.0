#include "ADirectives.hpp"

bool ADirectives::getDirective(const std::string &key, std::string &out_val) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = _directives.find(key);
	if (it == _directives.end())
		return false;
	out_val = it->second;
	return true;
}

bool ADirectives::setDirective(const std::string &key, const std::string &value) {
	if (_directives.find(key) != _directives.end())
		return false;
	_directives[key] = value;
	return true;
}
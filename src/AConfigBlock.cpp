#include "AConfigBlock.hpp"

bool AConfigBlock::getDirective(const std::string &key, std::string &out_val) const {
	std::map<std::string, std::string>::const_iterator	it;

	it = _directives.find(key);
	if (it == _directives.end())
		return false;
	out_val = it->second;
	return true;
}

void AConfigBlock::setDirective(const std::string &key, const std::string &value) {
	if (_directives.find(key) != _directives.end())
		return ;
	_directives[key] = value;
}

bool AConfigBlock::getMultiDirective(const std::string &key, std::vector<std::string> &out_val) const {
	std::map<std::string, std::vector<std::string>>::const_iterator	it;

	it = _multiDirectives.find(key);
	if (it == _multiDirectives.end())
		return false;
	out_val = it->second;
	return true;
}

void AConfigBlock::setMultiDirective(const std::string &key, const std::vector<std::string> &value) {
	if (_multiDirectives.find(key) != _multiDirectives.end())
		return ;
	_multiDirectives[key] = value;
}

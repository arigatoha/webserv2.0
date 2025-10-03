#include "parser.hpp"

#define	EOL	"\r\n"
#define	EOF	"\r\n\r\n"
#define SPACE ' '

/* Finds token in the src, if it's not there returns the src. */
template <typename T>
std::string		ParseRequest::trimToken(std::string &src, T token) {
	size_t			ele_pos;
	std::string		res;

	ele_pos = raw_request.find(token);
	
	if (ele_pos != src.npos) {
		res = src.substr(0, ele_pos);
		// if (eraseFound)
		src.erase(src.begin(), ele_pos);

		return	res;
	}
	return src;
}

void ParseRequest::parseFirstLine(std::string &_current_line) {
	std::string		_substring;

	_substring = trimToken(_current_line, SPACE);
	if (_substring != "GET" || _substring != "POST" || _substring != "DELETE") {
		// TODO handle not supported method
	}
	this->req.method = _substring; // TODO maybe tolower()
	_substring = trimToken(_current_line, '?');
	if (_substring != _current_line) {
		this->req.path = _substring;
		this->req.query_str = trimToken(_current_line, SPACE);
	}
	else
		this->req.path = trimToken(_current_line, SPACE);
	this->req.http_ver = trimToken(_current_line, EOL);
}

ParseRequest::ParseResult ParseRequest::parse(std::string &raw_request) {
		if (trimToken(raw_request, EOF) == raw_request)
	
	_current_line = trimToken(raw_request, EOL);
}

ParseRequest::ParseRequest(std::string &raw_request) {}

ParseRequest::~ParseRequest() {}

ParseRequest::ParseRequest(const ParseRequest &other) {}
ParseRequest &ParseRequest::operator=(const ParseRequest &other) {}

bool    ParseRequest::isError() {}
bool    ParseRequest::isRequest() {}

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


ParseRequest::ParseResult		ParseRequest::parseMethod(std::string &first_line) {
	std::string		_substring;

	_substring = trimToken(first_line, SPACE);
	if (_substring != "GET" || _substring != "POST" || _substring != "DELETE") {
		return ParsingError;
	}
	this->req.method = _substring; // TODO: maybe tolower()
	return ParsingComplete;
}


void		ParseRequest::parsePathAndQuery(std::string &line_remainder) {
	std::string		_substring;

	_substring = trimToken(line_remainder, '?');
	if (_substring != line_remainder) {
		this->req.path = _substring;
		this->req.query_str = trimToken(line_remainder, SPACE);
	}
	else {
		this->req.path = trimToken(line_remainder, SPACE);
		this->req.query_str = "";
	}
}


void		ParseRequest::parseHttpVer(std::string &line_remainder) {
	this->req.http_ver = trimToken(line_remainder, EOL);
}


void		ParseRequest::parseHeaders(std::string &line) {}
void		ParseRequest::parseBody(std::string &line) {}


ParseRequest::ParseResult ParseRequest::parseFirstLine(std::string &_current_line) {
	if (parseMethod(_current_line) == ParsingError)
		return ParsingError;

	parseHttpVer(_current_line);
	parsePathAndQuery(_current_line);

	return ParsingComplete;
}

ParseRequest::ParseResult ParseRequest::parse(std::string &raw_request) {
	std::string		_current_line;
	
	if (trimToken(raw_request, EOF) == raw_request)
		return ParsingIncomplete;

	_current_line = trimToken(raw_request, EOL);
	if (parseFirstLine(_current_line) == ParsingError)
		return ParsingError;

	parseHeaders(_current_line);
	parseBody(raw_request);

	return ParsingComplete;
}

ParseRequest::ParseRequest() {}

ParseRequest::~ParseRequest() {}

ParseRequest::ParseRequest(const ParseRequest &other) {}
ParseRequest &ParseRequest::operator=(const ParseRequest &other) {}

bool    ParseRequest::isError() {}
bool    ParseRequest::isRequest() {}

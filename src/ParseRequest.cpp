#include "ParseRequest.hpp"

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


ParseRequest::ParseResult		ParseRequest::parseMethod(std::string &first_line, std::string &method) {
	std::string		_substring;

	_substring = trimToken(first_line, SPACE);
	if (_substring != "GET" || _substring != "POST" || _substring != "DELETE") {
		return ParsingError;
	}
	method = _substring; // TODO: maybe tolower()
	return ParsingComplete;
}


void		ParseRequest::parsePathAndQuery(std::string &line_remainder, std::string &path, std::string &query_str) {
	std::string		_substring;

	_substring = trimToken(line_remainder, '?');
	if (_substring != line_remainder) {
		path = _substring;
		query_str = trimToken(line_remainder, SPACE);
	}
	else {
		path = trimToken(line_remainder, SPACE);
		query_str = "";
	}
}


void		ParseRequest::parseHttpVer(std::string &line_remainder, std::string &http_ver) {
	http_ver = trimToken(line_remainder, EOL);
}


void		ParseRequest::parseHeaders(std::string &line, std::map<std::string, std::string> &headers) {}
void		ParseRequest::parseBody(std::string &line, std::string &body) {}


ParseRequest::ParseResult ParseRequest::parseFirstLine(std::string &_current_line, HttpRequest &req) {
	if (parseMethod(_current_line, req.method) == ParsingError)
		return ParsingError;

	parseHttpVer(_current_line, req.http_ver);
	parsePathAndQuery(_current_line, req.path, req.query_str);

	return ParsingComplete;
}

ParseRequest::ParseResult ParseRequest::parse(std::string &raw_request, HttpRequest &req) {
	std::string		_current_line;
	
	if (trimToken(raw_request, EOF) == raw_request)
		return ParsingIncomplete;

	_current_line = trimToken(raw_request, EOL);
	if (parseFirstLine(_current_line, req) == ParsingError)
		return ParsingError;

	parseHeaders(_current_line, req.headers);
	parseBody(raw_request, req.body);

	return ParsingComplete;
}

ParseRequest::ParseRequest() {}

ParseRequest::~ParseRequest() {}

ParseRequest::ParseRequest(const ParseRequest &other) {}
ParseRequest &ParseRequest::operator=(const ParseRequest &other) {}

bool    ParseRequest::isError() {}
bool    ParseRequest::isRequest() {}

ParseRequest::ParseResult ParseRequest::parse_config(std::string &path, pConfig &config) {

}

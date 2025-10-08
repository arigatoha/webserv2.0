#include "ParseRequest.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>

#define	EOL	"\r\n"
#define	EOH	"\r\n\r\n"
#define SPACE ' '

/* Finds token in the src, if it's not there returns the src. */
template <typename T>
std::string		ParseRequest::trimToken(std::string &src, T token) {
	size_t			ele_pos;
	std::string		res;

	ele_pos = src.find(token);
	
	if (ele_pos != src.npos) {
		res = src.substr(0, ele_pos);
		// if (eraseFound)
		src.erase(src.begin(), ele_pos);

		return	res;
	}
	return src;
}


void		ParseRequest::parseMethod(std::string &first_line, std::string &method) {
	std::string		_substring;

	_substring = trimToken(first_line, SPACE);

	method = _substring; // TODO: maybe tolower()
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


void		ParseRequest::parseHeaders(std::string &request, std::map<std::string, std::string> &store_headers) {
	std::string		header_line;
	std::string		key;
	std::string		value;
	size_t			delim_pos;

	while(!request.empty()) {
		header_line = trimToken(request, EOL);
		if (header_line == EOL)
			break;
		delim_pos = request.find(':');
		if (delim_pos == request.npos)
			break;
		key = request.substr(0, delim_pos);
		value = request.substr(delim_pos + 1);
		key.erase(key.begin(), std::find_if(key.begin(), key.end(), [](unsigned char ch) { return !std::isspace(ch); }));		
		value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch) { return !std::isspace(ch); }));		
		store_headers[key] = value;
	}
}

void		ParseRequest::parseBody(std::string &line, std::string &body) {}


void		ParseRequest::parseFirstLine(std::string &_current_line, HttpRequest &req) {
	parseMethod(_current_line, req.method);
	parseHttpVer(_current_line, req.http_ver);
	parsePathAndQuery(_current_line, req.path, req.query_str);

}

ParseRequest::ParseResult ParseRequest::parse(const std::string &raw_request, HttpRequest &req) {
	std::string		_current_line;
	std::string		reqNoBody;
	size_t			eoh_pos;
	
	eoh_pos = raw_request.find(EOH);
	if (eoh_pos == raw_request.npos)
		return ParsingIncomplete;
	reqNoBody = raw_request.substr(0, eoh_pos + 2);

	_current_line = trimToken(reqNoBody, EOL);
	parseFirstLine(_current_line, req);
	
	parseHeaders(reqNoBody, req.headers);
	//check content-length header if exists

	parseBody("TODO", req.body);

	return ParsingComplete;
}

ParseRequest::ParseRequest() {}

ParseRequest::~ParseRequest() {}

ParseRequest::ParseRequest(const ParseRequest &other) {}
ParseRequest &ParseRequest::operator=(const ParseRequest &other) {}

bool    ParseRequest::isError() {}
bool    ParseRequest::isRequest() {}

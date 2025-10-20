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
		src.erase(0, ele_pos);

		return	res;
	}
	return src;
}


void		ParseRequest::parseMethod(std::string &first_line, HttpRequest &req) {
	std::string		_substring;

	_substring = trimToken(first_line, SPACE);

	req.setMethod(_substring); // TODO: maybe tolower()
}


void		ParseRequest::parsePathAndQuery(std::string &line_remainder, HttpRequest &req) {
	std::string		_substring;

	_substring = trimToken(line_remainder, '?');
	if (_substring != line_remainder) {
		req.setPath(_substring);
		req.setQuery(trimToken(line_remainder, SPACE));
	}
	else {
		req.setPath(trimToken(line_remainder, SPACE));
		req.setQuery("");
	}
}


void		ParseRequest::parseHttpVer(std::string &line_remainder, HttpRequest &req) {
	req.setVersion(trimToken(line_remainder, EOL));
}


void		ParseRequest::parseHeaders(std::string &request, HttpRequest &req) {
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
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		value = request.substr(delim_pos + 1);
		trimLeftWhitespace(key);
		trimLeftWhitespace(value);
		store_headers[key] = value; // potentially should check for duplicates
	}
}

void		ParseRequest::trimLeftWhitespace(std::string &to_trim) {
	const std::string WHITESPACE = " \r\t\n\f\v";
	size_t start = to_trim.find_first_not_of(WHITESPACE);

	if (start == to_trim.npos)
		to_trim.clear();
	else {
		to_trim.erase(0, start);
	}
}

void		ParseRequest::parseFirstLine(std::string &_current_line, HttpRequest &req) {
	parseMethod(_current_line, req);
	parseHttpVer(_current_line, req);
	parsePathAndQuery(_current_line, req);

}

ParseRequest::BodyState	ParseRequest::parseBody(size_t eoh_pos, const std::string &raw_request, HttpRequest &req) {
	size_t	body_size = 0;
	size_t	headers_size = eoh_pos + 4;

	if (method == "GET" || method == "DELETE" || method == "HEAD") {
		return BodyNotSent;
	}
	if (headers.find("content-length") == headers.end())
		return BodyError;

	body_size = std::stoul(headers.at("content-length"));

	if (body_size > 0 && raw_request.size() < headers_size + body_size) {
		return BodyIncomplete;
	}
	body = raw_request.substr(body_size + 1); // +1 ?
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
	
	parseHeaders(reqNoBody, req);
	
	if (parseBody(eoh_pos, raw_request, req) == BodyIncomplete)
		return ParsingIncomplete;

	return ParsingComplete;
}

ParseRequest::ParseRequest() {}

ParseRequest::~ParseRequest() {}

ParseRequest::ParseRequest(const ParseRequest &other) {}
ParseRequest &ParseRequest::operator=(const ParseRequest &other) {}

bool    ParseRequest::isError() {}
bool    ParseRequest::isRequest() {}

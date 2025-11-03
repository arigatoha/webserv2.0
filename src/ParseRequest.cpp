#include "ParseRequest.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <stdlib.h>

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
		std::cout << "qq" << std::endl;
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
		// std::cout << "1" << std::endl;
		header_line = trimToken(request, EOL);
		// std::cout << "2" << std::endl;
		if (header_line == EOL)
			break;
		// std::cout << "3" << std::endl;
		delim_pos = header_line.find(':');
		if (delim_pos == header_line.npos)
			break;
		// std::cout << "4" << std::endl;
		key = header_line.substr(0, delim_pos);
		// std::cout << "5" << std::endl;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		// std::cout << "6" << std::endl;
		value = header_line.substr(delim_pos + 1);
		// std::cout << "7" << std::endl;
		trimLeftWhitespace(key);
		// std::cout << "8" << std::endl;
		trimLeftWhitespace(value);
		// std::cout << "9" << std::endl;
		req.addHeader(key, value);
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
	const std::string &method = req.getMethod();

	if (method == "GET" || method == "DELETE" || method == "HEAD") {
		return BodyNotSent;
	}
	try
	{
		body_size = std::atoi(req.getHeader("content-length").c_str());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return BodyError;
	}
	if (body_size > 0 && raw_request.size() < headers_size + body_size) {
		return BodyIncomplete;
	}
	req.setBody(raw_request.substr(headers_size, body_size));// +1 ?
	return BodySent;
}

ParseRequest::ParseResult ParseRequest::parse(const std::string &raw_request, HttpRequest &req) {
	std::string		_current_line;
	std::string		reqNoBody;
	size_t			eoh_pos;

	// std::cout << "1" << std::endl;
	eoh_pos = raw_request.find(EOH);
	// std::cout << "2" << std::endl;
	if (eoh_pos == raw_request.npos)
		return ParsingIncomplete;
	// std::cout << "3" << std::endl;
	reqNoBody = raw_request.substr(0, eoh_pos + 2);
	// std::cout << "4" << std::endl;
	
	_current_line = trimToken(reqNoBody, EOL);
	// std::cout << "5" << std::endl;
	parseFirstLine(_current_line, req);
	// std::cout << "6" << std::endl;
	
	parseHeaders(reqNoBody, req);
	// std::cout << "7" << std::endl;
	
	if (parseBody(eoh_pos, raw_request, req) == BodyIncomplete)
		return ParsingIncomplete;
	
	// std::cout << "8" << std::endl;
	return ParsingComplete;
}

ParseRequest::ParseRequest() {}

ParseRequest::~ParseRequest() {}

ParseRequest::ParseRequest(const ParseRequest &other) { *this = other; }
ParseRequest &ParseRequest::operator=(const ParseRequest &other) {
	(void)other;
	return *this;
}

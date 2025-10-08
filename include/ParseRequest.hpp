#pragma once

#include <string>
#include <map>
#include "httpRequest.hpp"
#include "Config.hpp"

class ParseRequest {
	public:

		enum ParseResult {
			ParsingError,
			ParsingComplete,
			ParsingIncomplete,
		};

        ParseRequest();
        ~ParseRequest();

		ParseRequest(const ParseRequest &other);
		ParseRequest &operator=(const ParseRequest &other);

		ParseResult		parse(const std::string &raw_request, HttpRequest &req);

		bool    		isError();
		bool    		isRequest();

	private:
		void							parseFirstLine(std::string &_current_line, HttpRequest &req);

		template <typename T>
		std::string						trimToken(std::string &src, T token);

		void							parseMethod(std::string &first_line, std::string &method);
		void							parsePathAndQuery(std::string &line_remainder, std::string &path, std::string &query);
		void							parseHttpVer(std::string &line_remainder, std::string &http_ver);
		void							parseHeaders(std::string &line, std::map<std::string, std::string> &headers);
		void							parseBody(std::string &line, std::string &body);
		
};	

#pragma once

#include <string>
#include <map>


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

		static ParseResult		parse(std::string &raw_request);

		bool    		isError();
		bool    		isRequest();

	private:

		static ParseRequest::ParseResult		parseFirstLine(std::string &_current_line);

		template <typename T>
		static std::string						trimToken(std::string &src, T token);

		static ParseRequest::ParseResult		parseMethod(std::string &first_line);
		static void							parsePathAndQuery(std::string &line_remainder);
		static void							parseHttpVer(std::string &line_remainder);
		static void							parseHeaders(std::string &line);
		static void							parseBody(std::string &line);
		
};	

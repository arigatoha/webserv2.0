#pragma once

#include <string>
#include "Config.hpp"

struct Token {
	std::string value;
	size_t		line;
}

class ParseConfig {
	public:

		ParseConfig();
		~ParseConfig();

		ParseConfig(const ParseConfig &other);
		ParseConfig &operator=(const ParseConfig &other);

		void parse(const std::string &path, Config &config);

	private:
		std::vector<Token>				_tokens;
		size_t							_token_index;

		const Token									&getNextToken();
		const Token									&peekNextToken() const;
		bool										isAtEnd() const;


		void										parseBlock(AConfigBlock &block)
		ResponseCode								checkPath(const std::string &path);
		void										setErrorPage(std::string &error_code, const std::string &file);
		void										syntaxCheck(std::vector<std::string> &tokens);
		std::pair<std::string, std::string>			parseLocDirectives(std::vector<std::string> &tokens);
		Location									parseLocation(std::vector<std::string> &tokens);
		std::vector<Token>							tokenize(const std::string &path);
		std::string									safelyExtractRawStr(const std::string &path);
};

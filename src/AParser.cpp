#include "AParser.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <iostream>

#define	EOL	"\r\n"

std::string    AParser::safelyExtractRawStr(const std::string &path) {
	std::stringstream   buffer;
	struct stat file_stat;
	if (stat(path.c_str(), &file_stat) != 0) {
		std::cerr << "Error: Unable to open configuration file: " << strerror(errno) << std::endl;
		throw std::runtime_error("Could not open configuration file.");
	}
	if (!S_ISREG(file_stat.st_mode)) {
		std::cerr << "Error: Configuration file is not a regular file." << std::endl;
		throw std::runtime_error("Configuration file is not a regular file.");
	}

	std::ifstream file(path.c_str(), std::ios::binary);

	buffer << file.rdbuf();

	return buffer.str();
}

const Token		&AParser::getNextToken() {
	if (isAtEnd())
		throw std::runtime_error("Parsing error: Unexpected end of file.");
	return _tokens[_token_index++];
}
const Token		&AParser::peekNextToken() const {
	if (isAtEnd())
		throw std::runtime_error("Parsing error: Unexpected end of file.");
	return _tokens[_token_index];
}
bool			AParser::isAtEnd() const {
	return this->_token_index >= _tokens.size();

}

void    AParser::tokenize(const std::string &path) {
	Token                 		token;
	std::string                 raw_config;
	std::stringstream          	tokenStream;
	size_t						line;

	raw_config = safelyExtractRawStr(path);
	tokenStream.str(raw_config);

    _token_index = 0;
	line = 1;
	while (tokenStream >> token.value) {
		_tokens.push_back(token);
		if (token.value.find(EOL) != token.value.npos) {
			++line;
		}
	}
}

AParser &AParser::operator=(const AParser &other) {
	if (this != &other) {
		this->_tokens = other._tokens;
		this->_token_index = other._token_index;
	}
	return *this;
}

#include "ParseConfig.hpp"
#include "StringUtils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <errno.h>
#include <sys/stat.h>
#include <cstring>

#define	EOL	"\r\n"

std::string    ParseConfig::safelyExtractRawStr(const std::string &path) {
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

void	ParseConfig::syntaxCheck() {
	if (getNextToken().value == "server") {
		if (getNextToken().value == "{") {
			return;
		} else {
			std::cerr << "Error: Expected '{' after 'server' directive. On line " << peekNextToken().line << std::endl;
			throw std::runtime_error("wrong syntax in configuration file.");
		}
	} else {
		std::cerr << "Error: Configuration file must start with 'server' directive. On line " << peekNextToken().line << std::endl;
		throw std::runtime_error("wrong syntax in configuration file.");
	}
}

void ParseConfig::parse(const std::string &cfg_path, Config &config) {
	_tokens = tokenize(cfg_path);
	_token_index = 0;
	
	syntaxCheck();
	parseBlock(config);
}

// void		ParseConfig::parseServers() {
// 	Config	new_server_cfg;


// 	// TODO pushback to server cfgs vector
// }

const Token		&ParseConfig::getNextToken() {
	if (isAtEnd())
		throw std::runtime_error("Parsing error: Unexpected end of file.");
	return _tokens[_token_index++];
}
const Token		&ParseConfig::peekNextToken() const {
	if (isAtEnd())
		throw std::runtime_error("Parsing error: Unexpected end of file.");
	return _tokens[_token_index];
}
bool			ParseConfig::isAtEnd() const {
	return this->_token_index >= _tokens.size();

}

Location	ParseConfig::parseLocationBlock() {
	std::string key;
	Location	loc;
	
	loc.setPath(getNextToken().value);
	if (getNextToken().value != "{")
		throw std::runtime_error("wrong syntax in configuration file.");
	for (;;) {
		key = getNextToken().value;

		if (key == "}")
			break;
		else if (key == "error_page") {
			std::vector<std::string>	error_code;
			while (1) {
				error_code.push_back(getNextToken().value);
				if (error_code.back()[error_code.back().length()-1] == ';') {
					error_code.back().erase(error_code.back().length()-1);
					break;
				}
			}
			std::string		error_file = error_code.back();
			error_code.pop_back();
			while (!error_code.empty()) {
				loc.setErrorPage(error_code.back(), error_file);
				error_code.pop_back();
			}
		}
		else if (key == "limit_except") {
			loc.addLimitExceptRules("method", getNextToken().value);
			if (peekNextToken().value == "{") {
				getNextToken().value;
			}
			while (1) {
				std::string limKey = getNextToken().value;
				if (limKey == "}")
					break;
				std::string limVal = getNextToken().value;
				limVal.erase(limVal.length()-1);
				loc.addLimitExceptRules(limKey, limVal);
			}
		}
		else {
			std::vector<std::string>	value;
			while (1) {
				value.push_back(getNextToken().value);
				if (value.back()[value.back().length()-1] == ';') {
					value.back().erase(value.back().length()-1);
					break;
				}
			}
			if (value.size() != 1)
				loc.setMultiDirective(key, value);
			else
				loc.setDirective(key, value[0]);
		}
	}
	return loc;
}

void ParseConfig::parseBlock(AConfigBlock &block) {
	std::string							key;

	for (;;) {
		int i = 0;

		key = getNextToken().value;

		if (key == "}")
			return;
		if (key == "location" && peekNextToken().value.find('/') == 0) {
			Config	*serv_cfg = dynamic_cast<Config*>(&block);
			if (serv_cfg == NULL) {
				throw std::runtime_error("config error. location block not in the server directive.");
			}
			Location loc = parseLocationBlock();
			if (serv_cfg->addLocation(loc) == false) {
				throw std::runtime_error("config error. duplicate location.");
			}
		}
		else if (key == "limit_except")
			throw std::runtime_error("limit_except in the server block");
		else if (key == "error_page") {
			std::vector<std::string>	error_code;
			while (1) {
				error_code.push_back(getNextToken().value);
				if (error_code.back()[error_code.back().length()-1] == ';') {
					error_code.back().erase(error_code.back().length()-1);
					break;
				}
			}
			std::string		error_file = error_code.back();
			error_code.pop_back();
			while (!error_code.empty()) {
				block.setErrorPage(error_code.back(), error_file);
				error_code.pop_back();
			}
		}
		else {
			// std::cout << key << std::endl;
			std::vector<std::string>	value;
			while (1) {
				value.push_back(getNextToken().value);
				if (value.back()[value.back().length()-1] == ';') {
					value.back().erase(value.back().length()-1);
					break;
				}
			}
			if (value.size() != 1)
				block.setMultiDirective(key, value);
			else
				block.setDirective(key, value[0]);
		}
		++i;
	}
}

std::vector<Token>    ParseConfig::tokenize(const std::string &path) {
	std::vector<Token>    		tokens;
	Token                 		token;
	std::string                 raw_config;
	std::stringstream          	tokenStream;
	size_t						line;

	raw_config = safelyExtractRawStr(path);
	tokenStream.str(raw_config);
	
	line = 1;
	while (tokenStream >> token.value) {
		tokens.push_back(token);
		if (token.value.find(EOL) != token.value.npos) {
			++line;
		}
	}
	return tokens;
}

ParseConfig::ParseConfig() {}

ParseConfig::~ParseConfig() {}

ParseConfig::ParseConfig(const ParseConfig &other) {
	*this = other;
}

ParseConfig &ParseConfig::operator=(const ParseConfig &other) {
	if (this != &other) {
		this->_tokens = other._tokens;
		this->_token_index = other._token_index;
	}
	return *this;
}

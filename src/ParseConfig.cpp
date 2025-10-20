#include "ParseConfig.hpp"
#include "StringUtils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <errno.h>

#define	EOL	"\r\n"

std::string    ParseConfig::safelyExtractRawStr(const std::string &path) {
	std::stringstream   buffer;

	std::ifstream file(path.c_str(), std::ios::binary);

	buffer << file.rdbuf();

	return buffer.str();
}

std::pair<std::string, std::string>	ParseConfig::parseLocDirectives(std::vector<std::string> &tokens) {
	std::pair<std::string, std::string>	pair;
	std::string							key;
	std::string							value;

	key = tokens.front();
	tokens.erase(tokens.begin());
	value = tokens.front();
	tokens.erase(tokens.begin());
	while (tokens.front() != ";") {
		value.append(" " + tokens.front());
		tokens.erase(tokens.begin());
	}
	tokens.erase(tokens.begin());

	pair.first = key;
	pair.second = value;
	return pair;
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
	if (!isAtEnd())
		throw std::runtime_error("Parsing error: Unexpected end of file.");
	return _tokens[_token_index++];
}
const Token		&ParseConfig::peekNextToken() const {
	if (!isAtEnd())
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
			while (peekNextToken().value != ";")
				error_code.push_back(getNextToken().value);
			std::string		error_file = error_code.back();
			error_code.pop_back();
			while (!error_code.empty()) {
				loc.setErrorPage(error_code.back(), error_file);
				error_code.pop_back();
			}
		}
		else {
			std::vector<std::string>	value;
			while (peekNextToken().value != ";")
				value.push_back(getNextToken().value);
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
		else if (key == "error_page") {
			std::vector<std::string>	error_code;
			while (peekNextToken().value != ";")
				error_code.push_back(getNextToken().value);
			std::string		error_file = error_code.back();
			error_code.pop_back();
			while (!error_code.empty()) {
				block.setErrorPage(error_code.back(), error_file);
				error_code.pop_back();
			}
		}
		else {
			std::vector<std::string>	value;
			while (peekNextToken().value != ";")
				value.push_back(getNextToken().value);
			if (value.size() != 1)
				block.setMultiDirective(key, value);
			else
				block.setDirective(key, value[0]);
		}
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
		if (token.value == EOL)
			++line;
	}
	return tokens;
}

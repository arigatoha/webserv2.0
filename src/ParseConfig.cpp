#include "ParseConfig.hpp"
#include "StringUtils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <errno.h>

std::string    ParseConfig::safelyExtractRawStr(const std::string &path) {
	std::stringstream   buffer;

	std::ifstream file(path);

	buffer << file.rdbuf();

	return buffer.str();
}

Location	ParseConfig::parseLocation(std::vector<std::string> &tokens) {
	Location								loc;
	std::string								path;
	std::vector<std::string>				loc_tokens;
	std::pair<std::string, std::string>		keyValue;

	tokens.erase(tokens.begin()); // erase "location"
	path = tokens.front();
	tokens.erase(tokens.begin());
	loc.setPath(path);
	loc_tokens = StringUtils::extractSubVecOfStr(tokens);
	tokens.erase(tokens.begin(), tokens.begin() + loc_tokens.size());

	while (!loc_tokens.empty()) {
		keyValue = parseLocDirectives(loc_tokens);
		loc.setDirective(keyValue.first, keyValue.second);
	}

	return loc;
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
		} else {
			std::cerr << "Error: Expected '{' after 'server' directive." << std::endl;
			exit(EXIT_FAILURE);
		}
	} else {
		std::cerr << "Error: Configuration file must start with 'server' directive." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void ParseConfig::parse(const std::string &cfg_path, Config &config) {
	_tokens = tokenize(cfg_path);
	_token_index = 0;

	parseServers();
}

void		ParseConfig::parseServers() {
	Config	new_server_cfg;

	syntaxCheck();
	parseBlock(new_server_cfg);
	// TODO pushback to server cfgs vector
}

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

void ParseConfig::parseBlock(AConfigBlock &block) {
	std::string							key;
	std::string							value;


	for (;;) {
		key = getNextToken().value;

		if (key == "}")
			return;
		if (key == "location" && value.find('/') == 0 && peekNextToken().value == "{") {
			Location loc = parseLocation(tokens);

			if (config.addLocation(loc) == false) {
				std::cerr << "Error: Duplicate location '" << loc.getPath() << "'." << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else if (key == "error_page") {
			std::string error_code = tokens.front();
			setErrorPage(,  file); // TODO
		}
		else if (key == "index") {
			while (tokens.front() != ";") {
				config.addIndex(value);
				tokens.erase(tokens.begin());
			}
			tokens.erase(tokens.begin());
		}
		else if (key == "autoindex") {

		}
		// hardcode every necessary directive ;( TODO
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
	while (tokenStream >> token.line) {
		tokens.push_back(token);
		if (token.line == EOL)
			++line;
	}
	return tokens;
}

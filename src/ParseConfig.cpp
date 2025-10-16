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

void	ParseConfig::syntaxCheck(std::vector<std::string> &tokens) {
	if (tokens.front() == "server") {
		tokens.erase(tokens.begin());
		if (tokens.front() == "{") {
			tokens.erase(tokens.begin());
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
	std::vector<std::string>			tokens;
	std::string							key;
	std::string							value;

	tokens = tokenize(cfg_path);
	syntaxCheck(tokens);

	for (;;) {
		key = tokens.front();
		tokens.erase(tokens.begin());
		if (key == "}") {
			if (!tokens.empty()) {
				std::cerr << "Error: Unexpected tokens after closing '}'." << std::endl;
				exit(EXIT_FAILURE);
			}
			break;
		}
		value = tokens.front();
		tokens.erase(tokens.begin());
		if (key == "location" && value.find('/') == 0 && tokens.front() == "{") {
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

std::vector<std::string>    ParseConfig::tokenize(const std::string &path) {
	std::vector<std::string>    tokens;
	std::string                 token;
	std::string                 raw_config;
	std::stringstream          	tokenStream;

	raw_config = safelyExtractRawStr(path);
	tokenStream.str(raw_config);

	while (tokenStream >> token) {
		tokens.push_back(token);
	}
	return tokens;
}

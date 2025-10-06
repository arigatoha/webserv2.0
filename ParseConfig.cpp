#include "ParseConfig.hpp"
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <errno.h>

std::string    ParseConfig::safelyExtractRawStr(const std::string &path) {
    struct stat         st;
    std::stringstream   buffer;

    if (stat(path.c_str(), &st) == -1) {
        std::cerr << "stat: " << errno << std::endl; //TODO: check specific error
        exit(EXIT_FAILURE);
    }

    std::ifstream file(path);

    buffer << file.rdbuf();

    return buffer.str();
}

void ParseConfig::parse(const std::string &path, Config &config) {
    std::string     raw_config;
    
    raw_config = safelyExtractRawStr(path);
    for (size_t i = 0; i < raw_config.size(); ++i) {
        if (raw_config[i] == '\n' || raw_config[i] == '\t' || raw_config[i] == '\r') {
            raw_config[i] = ' ';
        }
    }

}

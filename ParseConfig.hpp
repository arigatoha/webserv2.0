#pragma once

#include <string>
#include "Config.hpp"

class ParseConfig {
    public:

        ParseConfig();
        ~ParseConfig();

        ParseConfig(const ParseConfig &other);
        ParseConfig &ParseConfig::operator=(const ParseConfig &other);

        void parse(const std::string &path, Config &config);

    private:

        std::string    safelyExtractRawStr(const std::string &path);
};

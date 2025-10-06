#pragma once

#include <string>

class ParseConfig {
    public:

        ParseConfig();
        ~ParseConfig();

        ParseConfig(const ParseConfig &other);
        ParseConfig &ParseConfig::operator=(const ParseConfig &other);

        void parse(const std::string &path);

    private:


};

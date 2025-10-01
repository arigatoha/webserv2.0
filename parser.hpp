#pragma once

#include <string>

class ParseRequest {
    public:
        std::string     request;

        ParseRequest(std::string &rough_request);
        ~ParseRequest();

        ParseRequest(const ParseRequest &other);
        ParseRequest &operator=(const ParseRequest &other);

        bool    isError();
        bool    isRequest();

};

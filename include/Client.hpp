#pragma once

#include <string>
#include "httpRequest.hpp"
#include "ParseRequest.hpp"

class Client {
	public:

		std::string request_buffer;
		
        ssize_t		keep_alive_timer;

        ParseRequest    parser;
		HttpRequest     req;

		Client();
		~Client();

		Client(const Client &other);
		Client &operator=(const Client &other);
};

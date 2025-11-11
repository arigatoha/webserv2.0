#pragma once

#include <string>
#include "httpRequest.hpp"
#include "ParseRequest.hpp"

class Server;

class Client {
	public:

		Client();
		~Client();

		Client(const Client &other);
		Client &operator=(const Client &other);

		const HttpRequest &req() const;
		
		void processNewData(const char *data, ssize_t len, Server *server);

	private:

		std::string		_request_buffer;
        int				_keep_alive_timer;
        ParseRequest    _parser;
		HttpRequest     _req;

};

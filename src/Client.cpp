#include "Client.hpp"
#include <iostream>
#include <map>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include "server.hpp"

Client::Client() : _request_buffer(""), _keep_alive_timer(0), _parser(), _req() {}

Client::~Client() {}

Client::Client(const Client &other) { *this = other; }

Client &Client::operator=(const Client &other) {
    if (this != &other) {
        this->_request_buffer = other._request_buffer;
        this->_keep_alive_timer = other._keep_alive_timer;
        this->_parser = other._parser;
        this->_req = other._req;
    }
    return *this;
}

const HttpRequest &
Client::req() const {
    return _req;
}


void
Client::processNewData(const char *data, ssize_t len, Server *server) {
    _request_buffer.reserve(len);
    _request_buffer.append(data);
    // TODO potentially dynamically allocate memore if keep_alive
    if (_parser.parse(_request_buffer, _req) == ParseRequest::ParsingComplete)
        server->_handler.handle(_config, client.req(), client_fd);
}

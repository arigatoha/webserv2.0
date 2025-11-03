#include "Client.hpp"
#include <iostream>
#include <map>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>

Client::Client() : request_buffer(""), keep_alive_timer(0), parser(), req() {}

Client::~Client() {}

Client::Client(const Client &other) { *this = other; }

Client &Client::operator=(const Client &other) {
    if (this != &other) {
        this->request_buffer = other.request_buffer;
        this->keep_alive_timer = other.keep_alive_timer;
        this->parser = other.parser;
        this->req = other.req;
    }
    return *this;
}

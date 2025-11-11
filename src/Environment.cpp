#include "Environment.hpp"
#include <algorithm>
#include <stdexcept>

Environment::Environment(const Client &client, const Server &server, const Location &loc) :
    _client(client),
    _server(server),
    _loc(loc),
    _cenv(NULL) {}

Environment::~Environment() {
    delete[] _cenv;
}

Environment::Environment(const Environment &other) :
    _client(other._client),
    _server(other._server),
    _loc(other._loc),
    _cenv(NULL) // shallow copy
{ *this = other; }

Environment &
Environment::operator=(const Environment &other) {
    if (this != &other) {
        _vsenv = other._vsenv;
    }
    return *this;
}

void
Environment::build() {
    _vsenv.reserve(dfl_size + _client.req().headers().size());

    const HttpRequest &req = _client.req();

    append("SERVER_PORT", _server.port());
    append("REQUEST_METHOD", req.getMethod());
    // if (req.getQuery().size()) TODO: PATH_INFO and PATH_TRANSLATED
    append("");

    try {
       append("CONTENT_TYPE", req.getHeader("content-type"));
    } catch (std::out_of_range &) {}
    try {
       append("CONTENT_LENGTH", req.getHeader("content-length"));
    } catch (std::out_of_range &) {}

    append(req.headers());

    _build_cenv();
}

void
Environment::_build_cenv() {
    return;
}

void
Environment::append(const std::string &key, const std::string &val) {
    _vsenv.push_back(env_str(key, val));
}

void
Environment::append(const std::map<std::string, std::string> &hdrs) {
    std::string         key;

    for (std::map<std::string, std::string>::const_iterator it = hdrs.begin(); it != hdrs.end(); ++it) {
        key = it->first;
        std::transform(key.begin(), key.end(), key.begin(), trans_key(key));
        if (key == "CONTENT_TYPE" || key == "CONTENT_LENGTH")
            continue;
        key = "HTTP_" + key;
        _vsenv.push_back(env_str(key, it->second));
    }
}

std::string
Environment::trans_key(std::string &key) {
    for (int i = 0; i < key.size(); ++i) {
        key[i] = (key[i] == '-') ? '_' : std::toupper(key[i]);
    }
    return key;
}

std::string
Environment::env_str(const std::string &key, const std::string &val) {
    return key + '=' + val;
}

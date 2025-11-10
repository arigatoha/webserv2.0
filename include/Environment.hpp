#pragma once

#include <vector>
#include <string>
#include "httpRequest.hpp"
#include "server.hpp"
#include "Client.hpp"

class Environment {
    public:

        Environment(const Client &client, const Server &server);
        ~Environment();

        Environment &operator=(const Environment &other);
        Environment(const Environment &other);

        void build(const HttpRequest &req) const;
    
    private:
        
        std::vector<std::string>    _vsenv;
        char                        **_cenv;
        const Client                &_client;
        const Server                &_server;

        void            append(const std::string &key, const std::string &val);
        void            append(const std::map<std::string, std::string> &);
        std::string     env_str(const std::string &key, const std::string &val);
        std::string     trans_key(std::string &key);
};

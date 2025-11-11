#pragma once

#include <vector>
#include <string>
#include "httpRequest.hpp"
#include "server.hpp"
#include "Client.hpp"

class Environment {
    public:

        Environment(const Client &client, const Server &server, const Location &loc);
        ~Environment();

        Environment &operator=(const Environment &other);
        Environment(const Environment &other);

        void build();
  
        static const size_t     dfl_size = 17; // -2 in headers cuz of content len and type?
        static const size_t     const_size = 2;
        char const              *static_env[const_size] = {
			"GATEWAY_INTERFACE=CGI/1.1", "SERVER_SOFTWARE=webserv/1.0"};


    private:
        
        std::vector<std::string>    _vsenv;
        char                        **_cenv;
        const Client                &_client;
        const Server                &_server;
		const Location				&_loc;

        void            append(const std::string &key, const std::string &val);
        void            append(const std::map<std::string, std::string> &);
        std::string     env_str(const std::string &key, const std::string &val);
        std::string     trans_key(std::string &key);

        void            _build_cenv();

};

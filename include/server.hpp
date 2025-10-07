#pragma once

#include "Client.hpp"
#include "Config.hpp"
#include <sys/epoll.h>
#include "ParseConfig.hpp"

class Server {
    public:

        Server();
        ~Server();

        Server(const Server &other);
        Server &operator=(const Server &other);

        void    run(const std::string &cfg_file);

    private:

        int                     listen_sock;
        int                     epoll_fd;
        std::map<int, Client>   clients;
        Config                  config;
        ParseConfig				ConfigParser;

        void    init_epoll(epoll_event *ev);
        void    init_sockets(const char *port);
        void    run_event_loop(epoll_event *ev);
        void    hints_init(struct addrinfo *hints);
        void    handle_new_connection();
        void    handle_client_event(int client_fd, int epoll_fd, std::map<int, Client> &clients);
        void    disconnect_client(int client_fd, int epoll_fd, std::map<int, Client> &clients);
        void    handle_parsed_request(const HttpRequest &req, int client_fd);
        void    send_response(int client_fd, const std::string &response);

};

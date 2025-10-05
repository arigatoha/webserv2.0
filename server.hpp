#pragma once

#include "Client.hpp"
#include <sys/epoll.h>

class Server {
    public:

        int                 listen_sock;
        int                 epoll_fd;
        std::map<int, Client> clients;
        //config?

        Server();
        ~Server();

        Server(const Server &other);
        Server &operator=(const Server &other);

        void    run();

    private:

        void    init_epoll(epoll_event *ev);
        void    init_sockets();
        void    run_event_loop(epoll_event *ev);
        void    hints_init(struct addrinfo *hints);
        void    handle_new_connection();
        void    handle_client_event(int client_fd, int epoll_fd, std::map<int, Client> &clients);
        void    disconnect_client(int client_fd, int epoll_fd, std::map<int, Client> &clients);
        void    handle_parsed_request(const HttpRequest &req, int client_fd);
        void    send_response(int client_fd, const std::string &response);

};

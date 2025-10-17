#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstdio>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <errno.h>
#include <iostream>
#include <map>
#include "ParseConfig.hpp"
#include "Client.hpp"
#include "server.hpp"

#define BUF_SIZE 4096
#define LISTEN_BACKLOG 50
#define MAX_EVENTS 10
#define MAX_REQUEST_SIZE 8192

void	Server::hints_init(struct addrinfo *hints) {
	memset(&hints, 0, sizeof(hints));
	hints->ai_family = AF_UNSPEC;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
	hints->ai_protocol = 0;
	hints->ai_canonname = NULL;
	hints->ai_next = NULL;
}

void	Server::disconnect_client(int client_fd, int epoll_fd, std::map<int, Client> &clients) {
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) == -1) {
		fprintf(stderr, "epoll_ctl (DEL): %s\n", strerror(errno));
	}
	close(client_fd);
	clients.erase(client_fd);
	std::cout << "Client on fd " << client_fd << " disconnected." << std::endl;
}

void	Server::handle_client_event(int client_fd, int epoll_fd, std::map<int, Client> &clients) {
	ssize_t				bytes_read;
	char				temp_buf[BUF_SIZE];
	std::string			&client_req = clients[client_fd].request_buffer;
	Client &client = clients[client_fd];

	bytes_read = recv(client_fd, temp_buf, BUF_SIZE, 0);
	if (bytes_read > 0) {
		client_req.append(temp_buf);
		if (client_req.size() > MAX_REQUEST_SIZE) {
			send(client_fd, "HTTP/1.1 413 Payload Too Large\r\n\r\n", 32, 0); // TODO
			this->disconnect_client(client_fd, epoll_fd, clients);
			return;
		}
		if (client.parser.parse(client_req, client.req) == ParseRequest::ParsingComplete) {
			std::string response = this->handle_parsed_request(clients[client_fd].req, client_fd);
			std::cout << "Request parsed successfully:\n" << client_req << std::endl;
			client_req.clear();
			return;
		}
		else {
			return;
		}
	}
	else if (bytes_read == 0) {
		this->disconnect_client(client_fd, epoll_fd, clients);
	}
	else {
		fprintf(stderr, "Client: %s port\n", strerror(errno));
		this->disconnect_client(client_fd, epoll_fd, clients);
	}
}

void Server::run(const std::string &cfg_file) {
	struct epoll_event		ev;

	ConfigParser.parse(cfg_file, this->config);
	this->init_sockets(this->config.getPort());

	if (listen(this->listen_sock, LISTEN_BACKLOG) == -1) {
		fprintf(stderr, "listen: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	this->init_epoll(&ev);
	this->run_event_loop(&ev);
}

void	Server::init_sockets(const char *port) {
	struct addrinfo			*result, *rp;
	struct addrinfo			hints;
	int						s, optval_int;
	
	this->hints_init(&hints);
	s = getaddrinfo(NULL, port, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	optval_int = 1;
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		this->listen_sock = socket(rp->ai_family, rp->ai_socktype,
					rp->ai_protocol);
		if (this->listen_sock == -1)
			continue;

		if (setsockopt(this->listen_sock, SOL_SOCKET, SO_REUSEADDR, &optval_int, sizeof(int)) == -1) {
			fprintf(stderr, "setsockopt: %s\n", strerror(errno));
			exit(EXIT_FAILURE);    
		}

		if (bind(this->listen_sock, rp->ai_addr, rp->ai_addrlen) == 0)
			break;
		
		close(this->listen_sock);
	}
	freeaddrinfo(result);

	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}

}

void	Server::init_epoll(epoll_event *ev) {
	this->epoll_fd = epoll_create1(0);
	if (this->epoll_fd == -1) {
		fprintf(stderr, "epoll_create1: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	ev->events = EPOLLIN;
	ev->data.fd = this->listen_sock;
	if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->listen_sock, ev) == -1) {
		fprintf(stderr, "epoll_ctl: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void	Server::run_event_loop(epoll_event *ev) {
	int                 	conn_sock, nfds;
	struct epoll_event		events[MAX_EVENTS];

	for (;;) {
		nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			fprintf(stderr, "epoll_wait: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < nfds; ++i) {
			if (events[i].data.fd == this->listen_sock) {
				struct	sockaddr_storage	client_addr;
				socklen_t	clientaddr_len = sizeof(client_addr);

				conn_sock = accept(this->listen_sock, reinterpret_cast<sockaddr*>(&client_addr), &clientaddr_len);
				if (conn_sock == -1) {
					if (errno == EAGAIN || errno == EWOULDBLOCK)
						continue;
					else {
						fprintf(stderr, "accept: %s\n", strerror(errno));
						continue;
					}

				}
				ev->events = EPOLLIN;
				ev->data.fd = conn_sock;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_sock, ev) == -1) {
					fprintf(stderr, "epoll_ctl (conn_sock): %s\n", strerror(errno));
					close(conn_sock);
				}
				else {
					clients[conn_sock] = Client();
					std::cout << "New connection on fd " << conn_sock << std::endl;
				}
			}
			else {
				handle_client_event(events[i].data.fd, epoll_fd, clients);
			}
		}
	}
}

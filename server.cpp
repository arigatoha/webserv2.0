#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <cstdlib>
#include <cstdio>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <errno.h>

#define BUF_SIZE 4096
#define LISTEN_BACKLOG 50
#define MAX_EVENTS 10
#define MAX_REQUEST_SIZE 8192

void	hints_init(struct addrinfo *hints) {
	memset(&hints, 0, sizeof(hints));
	hints->ai_family = AF_UNSPEC;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
	hints->ai_protocol = 0;
	hints->ai_canonname = NULL;
	hints->ai_next = NULL;
}

void	disconnect_client();

void	do_use_fd(int client_fd) {
	char				temp_buf[BUF_SIZE];
	std::string			client_req;

	for (ssize_t bytes_read = 0;;) {
		bytes_read = recv(client_fd, temp_buf, BUF_SIZE, 0);
		if (bytes_read > 0) {
			client_req.append(temp_buf);
			if (client_req.size() > MAX_REQUEST_SIZE) {
				// TODO send error 413 and close
			}
			parseRequest();
		}
		else if (bytes_read == 0) {
			disconnect_client();
		}
		else {
			fprintf(stderr, "Client: %s port\n", strerror(errno));
			disconnect_client(); //TODO
		}
	}
}

int main(int argc, char *argv[]) {
	int                 listen_sock, conn_sock, s, epoll_fd, nfds, optval_int;
	struct addrinfo		hints;
	struct addrinfo		*result, *rp;
	socklen_t			addr_len;
	struct epoll_event	ev, events[MAX_EVENTS];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	hints_init(&hints);

	s = getaddrinfo(argv[1], NULL, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	optval_int = 1;
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		listen_sock = socket(rp->ai_family, rp->ai_socktype,
					rp->ai_protocol);
		if (listen_sock == -1)
			continue;

		if (setsockopt(listen_sock, IPPROTO_TCP, SO_REUSEADDR, &optval_int, sizeof(int)) == -1) {
			fprintf(stderr, "setsockopt: %s\n", strerror(errno));
			exit(EXIT_FAILURE);    
		}

		if (bind(listen_sock, rp->ai_addr, rp->ai_addrlen) == 0)
			break;
		
		close(listen_sock);
	}
	freeaddrinfo(result);

	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}

	addr_len = sizeof(rp);

	if (listen(listen_sock, LISTEN_BACKLOG) == -1) {
		fprintf(stderr, "listen: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1) {
		fprintf(stderr, "epoll_create1: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
		fprintf(stderr, "epoll_ctl (listen_sock): %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (;;) {
		nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			fprintf(stderr, "epoll_wait: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < nfds; ++i) {
			if (events[i].data.fd == listen_sock) {
				conn_sock = accept(listen_sock, rp->ai_addr, &rp->ai_addrlen);
				if (conn_sock == -1) {
					fprintf(stderr, "accept: %s\n", strerror(errno));
					if (errno == EAGAIN || errno == EWOULDBLOCK || errno == ECONNABORTED)
						continue;
					else
						exit(EXIT_FAILURE);
				}
				ev.events = EPOLLIN | EPOLLOUT;
				ev.data.fd = conn_sock;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
					fprintf(stderr, "epoll_ctl (conn_sock): %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
			}
			else {
				do_use_fd(events[i].data.fd); // TODO fork
			}
		}
	}
}
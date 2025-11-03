#include "server.hpp"
#include "Client.hpp"
#include <cstdlib>

int main(int argc, char *argv[]) {
    Server  webserv;

    if (argc != 2)
        exit(EXIT_FAILURE);
    
    webserv.run(argv[1]);
}

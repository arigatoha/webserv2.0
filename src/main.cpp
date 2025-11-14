#include "server.hpp"
#include "Client.hpp"
#include <cstdlib>
#include "Environment.hpp"

int main(int argc, char *argv[], char **envp) {
    Server  webserv;

    if (argc != 2)
        exit(EXIT_FAILURE);
    
    // Environment::init_env(envp);
    
    Environment::_parent_env = envp;
    Environment::_parent_env_size = 0;
    if (envp) {
        while (envp[Environment::_parent_env_size])
            ++Environment::_parent_env_size;
    }

    webserv.run(argv[1]);
}

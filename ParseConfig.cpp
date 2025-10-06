#include "ParseConfig.hpp"
#include <sys/stat.h>

void ParseConfig::parse(const std::string &path) {
    struct stat     st;
    
    stat(path.c_str(), &st)
}

#pragma once

#include <string>
#include <vector>
#include <map>

namespace StringUtils {

    std::vector<std::string>    split(const std::string &s, char delimiter);
    std::vector<std::string>    extractSubVecOfStr(const std::vector<std::string> &src);

};

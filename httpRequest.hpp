#pragma once

#include <string>
#include <map>

struct HttpRequest {
	std::string							method;
	std::string							path;
	std::string							query_str;
	std::string							http_ver;
	std::string							body;
	std::map<std::string, std::string>	headers;
};

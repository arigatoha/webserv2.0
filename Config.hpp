#pragma once

#include <string>
#include <map>
#include "ParseConfig.hpp"

struct location {
    std::string     root;
    bool            autoindex;
};

class Config {
    ParseConfig     parser;
    
    std::string     server_names  = "default.org www.default.org";

	int listen_port		=				8080;

	std::string index=						"index.html";

	ssize_t     client_max_body_size	=	100000;

	std::string     error_page 404				/404.html;
	// error_page 500 502 503 504	/50x.html;

    std::string     allowed_methods;

	// limit_except GET {
			// allow	192.168.1.0/32;
			// deny	all;
	// }

    std::map<std::string, location> locations;

	// location / {
		// root	/;
		// index	index.html;
	// }
// 
	// location /kapouet {
		// root		/tmp/www;
		// autoindex	on;
		// cgi			.php;
	// }
// 
	// location /images {
		// autoindex	off;
	// }

};

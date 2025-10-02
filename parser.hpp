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

class ParseRequest {
	public:

		HttpRequest     req;


		ParseRequest(std::string &rough_request);
		~ParseRequest();

		ParseRequest(const ParseRequest &other);
		ParseRequest &operator=(const ParseRequest &other);



		bool    		isError();
		bool    		isRequest();

	private:

		void	parseFirstLine(std::string &_current_line);

		template <typename T>
		std::string		trimToken(std::string &src, T token);

		
};

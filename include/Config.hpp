#pragma once

#include <string>
#include <map>
#include <vector>
#include "Location.hpp"

enum ErrorCode {
	create_200 = 0,
	create_403,
	create_404,
	create_500
};

class Config : public ADirectives {
	public:

		Config();
		~Config();

		Config(const Config &other);
		Config &operator=(const Config &other);

		// const std::vector<Location>		&getLocations() const;
		bool							addLocation(const Location &loc);
		const char					*getPort() const { return _directives.at("listen").c_str(); }
		const std::string			getCustomErrorPage(ErrorCode code) const;
		
	private:

		void    	setError_page(const std::string &value);

    	std::map<int, std::string>			error_pages;
		std::vector<Location>				_locations;

};

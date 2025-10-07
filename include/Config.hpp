#pragma once

#include <string>
#include <map>
#include <vector>
#include "Location.hpp"

class Config : public ADirectives {
	public:

		Config();
		~Config();

		Config(const Config &other);
		Config &Config::operator=(const Config &other);

		// const std::vector<Location>		&getLocations() const;
		bool							addLocation(const Location &loc);
		const char					*getPort() const { return _directives.at("listen").c_str(); }
		
	private:

		void    	setError_page(const std::string &value);

    	std::map<int, std::string>			error_pages;
		std::vector<Location>				_locations;

};

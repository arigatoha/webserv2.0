#pragma once

#include <string>
#include <map>
#include <vector>
#include "Location.hpp"

class Config : public AConfigBlock {
	public:

		Config();
		~Config();

		Config(const Config &other);
		Config &operator=(const Config &other);

		const std::vector<Location>		&getLocations() const;
		bool							addLocation(const Location &loc);
		bool							getPort(const std::string &key, std::string &out_val) const;

		bool						getErrorPage(int code, std::string &errorPage) const;
	private:

		void    					setError_page(const std::string &value);

    	std::map<int, std::string>			error_pages;
		std::vector<Location>				_locations;

};

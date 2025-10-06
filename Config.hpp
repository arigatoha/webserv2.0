#pragma once

#include <string>
#include <map>
#include <vector>

class Location {
	public:

		const bool		getDirective(const std::string &key, std::string &out_val) const;

	private:

		std::map<std::string, std::string>	_directives;
};

class Config {
	public:

		Config();
		~Config();

		Config(const Config &other);
		Config &Config::operator=(const Config &other);

		const std::vector<Location>		&getLocations() const;
		bool							getDirective(const std::string &key, std::string &out_val) const;

		bool							setDirective(const std::string &key, const std::string &value);

	private:

		void    	setError_page(const std::string &value);


    	std::map<std::string, std::string>	_simple_directives;
    	std::map<int, std::string>			error_pages;
		std::vector<Location>				_locations;

};

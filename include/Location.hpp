#pragma once

#include <string>
#include <map>
#include "ADirectives.hpp"

class Location : public ADirectives {
	public:

		const std::string	&getPath(void) const;
		void				setPath(const std::string &path);
	private:

		std::string							_path;
};

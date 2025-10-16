#pragma once

#include <string>
#include <map>
#include "AConfigBlock.hpp"

class Location : public AConfigBlock {
	public:

		const std::string	&getPath(void) const;
		void				setPath(const std::string &path);
	private:

		std::string							_path;
};

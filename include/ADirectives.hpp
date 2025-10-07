#pragma once

#include <string>
#include <map>

class ADirectives {
    public:
		virtual ~ADirectives() {}
	
		bool				getDirective(const std::string &key, std::string &out_val) const;
        bool				setDirective(const std::string &key, const std::string &value);

	protected:
		ADirectives() {}
		ADirectives(const ADirectives &other) { (void)other; }
		ADirectives &operator=(const ADirectives &other) { (void)
			other; return *this; }
		
		std::map<std::string, std::string>	_directives;
	};
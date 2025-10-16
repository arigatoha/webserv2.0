#pragma once

#include <string>
#include <map>
#include <vector>

class ADirectives {
    public:
		virtual ~ADirectives() {}
	
		bool						getDirective(const std::string &key, std::string &out_val) const;
        bool						setDirective(const std::string &key, const std::string &value);
		void						addIndex(const std::string &value);
		bool						getIndexes(const std::vector<std::string> &out_indexes) const;
	protected:
		ADirectives() {}
		ADirectives(const ADirectives &other) { (void)other; }
		ADirectives &operator=(const ADirectives &other) { (void)
			other; return *this; }
		
		std::map<std::string, std::string>	_directives;
		std::vector<std::string>			_indexes;
	};
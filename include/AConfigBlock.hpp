#pragma once

#include <string>
#include <map>
#include <vector>

class AConfigBlock {
    public:
		virtual ~AConfigBlock() {}
	
		bool						getDirective(const std::string &key, std::string &out_val) const;
		bool						getMultiDirective(const std::string &key, std::vector<std::string> &out_val) const;
        void						setDirective(const std::string &key, const std::string &value);
        void						setMultiDirective(const std::string &key, const std::vector<std::string> &value);
	protected:
		AConfigBlock() {}
		AConfigBlock(const AConfigBlock &other) { (void)other; }
		AConfigBlock &operator=(const AConfigBlock &other) { (void)
			other; return *this; }
		
		std::map<std::string, std::string>					_directives;
		std::map<std::string, std::vector<std::string>>		_multiDirectives;
	};
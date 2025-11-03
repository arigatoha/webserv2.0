#pragma once

#include <string>
#include <map>
#include <vector>

struct limitExceptRules {
	std::vector<std::string>	_methods;
	std::vector<std::string>	_allow;
	std::vector<std::string>	_deny;
};

class AConfigBlock {
    public:
		virtual ~AConfigBlock() {}

		bool						getErrorPage(int error_code, std::string &out_val) const;
		bool						getIndexes(std::vector<std::string> &out_val) const;
		bool						isAutoindexOn() const;

		bool						getDirective(const std::string &key, std::string &out_val) const;
		bool						getMultiDirective(const std::string &key, std::vector<std::string> &out_val) const;

		virtual void						setDirective(const std::string &key, const std::string &value);
        virtual void						setMultiDirective(const std::string &key, const std::vector<std::string> &value);
		virtual void						setErrorPage(const std::string &error_code, const std::string &file);
		virtual void						addLimitExceptRules(const std::string &key, const std::string &value);

	protected:

		AConfigBlock() {}
		AConfigBlock(const AConfigBlock &other) { (void)other; }
		AConfigBlock &operator=(const AConfigBlock &other) { (void)
			other; return *this; }

		
		std::map<std::string, std::string>					_error_pages;
		std::map<std::string, std::string>					_directives;
		std::map<std::string, std::vector<std::string> >	_multiDirectives;
		limitExceptRules									_rules;
	};
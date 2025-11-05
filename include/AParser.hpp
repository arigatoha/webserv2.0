#pragma once

#include <string>
#include <vector>

struct Token {
	std::string value;
	size_t		line;
};

class AParser {
public:
    virtual ~AParser() = default;

    const Token									&getNextToken();
    const Token									&peekNextToken() const;
    void										tokenize(const std::string &path);

    AParser &operator=(const AParser &other);

private:
        std::vector<Token>				_tokens;
		size_t							_token_index;
	
		bool										isAtEnd() const;
		std::string									safelyExtractRawStr(const std::string &path);
};

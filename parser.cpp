#include "parser.hpp"

ParseRequest::ParseRequest(std::string &rough_request) {}
ParseRequest::~ParseRequest() {}

ParseRequest::ParseRequest(const ParseRequest &other) {}
ParseRequest &ParseRequest::operator=(const ParseRequest &other) {}

bool    ParseRequest::isError() {}
bool    ParseRequest::isRequest() {}

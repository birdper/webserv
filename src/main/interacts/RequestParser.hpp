#pragma once

#include <iostream>
#include <sstream>
#include <map>

#include "Request.hpp"

class RequestParser {

public:
	Request* parse(const std::string& data);

private:
	bool parseStartLine(std::istringstream& line, Request* request);

	void parseHeaders(std::istringstream& line, Request* request);
	void parseMethod(Request* request, const std::string& method) const;
	void parseUri(Request* request, const std::string& uri) const;
	bool parseHttpVersion(Request* request, const std::string& httpVersion) const;
	void parseBody();
};
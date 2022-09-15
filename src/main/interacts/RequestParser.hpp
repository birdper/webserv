#pragma once

#include <iostream>
#include <sstream>
#include <map>

#include "Utils.hpp"
#include "Constants.hpp"
#include "Request.hpp"
#include "ParseRequestException.hpp"


class RequestParser {

public:
	Request* parse(const string& requestString);

private:
	void parseStartLine(const string& line, Request& request);

	void parseHeaders(std::istringstream& iss, Request& request);
	void parseMethod(Request& request, const string& httpMethod) const;
	void parseUri(Request& request, const string& uri) const;
	void parseHttpVersion(Request& request, const string& httpVersion) const;
	void parseBody(Request& request);
};
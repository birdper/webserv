#pragma once

#include <iostream>
#include <sstream>
#include <map>

#include "Utils.hpp"
#include "Constants.hpp"
#include "Request.hpp"
#include "../exception/ParseRequestException.hpp"
#include "Client.hpp"

class RequestParser {

public:
	void parse(Request& request, Client& client) const;

private:
	void parseStartLine(const string& line, Request& request) const;
	void parseHeaders(std::vector<string>& headerLines, Request& request) const;
	void parseMethod(Request& request, const string& httpMethod) const;
	void parseUri(Request& request, const string& uri) const;
	void parseHttpVersion(Request& request, const string& httpVersion) const;
	bool isHasHeaders(const Request& request) const;
    void parseChunked(Request& request, Client& client) const;
    void parseBodyContent(Request& request, const string& buffer, Client& client) const;
	void printRequest(const std::vector<string>& headerLines) const;
};

#pragma once

#include <iostream>
#include <sstream>
#include <map>

#include "Utils.hpp"
#include "Constants.hpp"
#include "Request.hpp"
#include "../exception/ParseRequestException.hpp"


class RequestParser {

public:
	void parse(const string& requestBuffer, Request& request) const;

private:
	void parseStartLine(const std::string& line, Request& request) const;

	void parseHeaders(std::vector<string>& headerLines, Request& request) const;
	void parseMethod(Request& request, const string& httpMethod) const;
	void parseUri(Request& request, const string& uri) const;
	void parseHttpVersion(Request& request, const string& httpVersion) const;
	void parseBody(Request& request, const string& clientBuffer) const;
    bool isHasHeaders(const Request& request) const;
    void parseChunked(Request& request) const;
    void parseContent(Request& request, const string& clientBuffer) const;
    long ContentLengthToInt(const string& contentLength) const;
    void parsePost(Request& request) const;
};

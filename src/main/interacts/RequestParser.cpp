#include "RequestParser.hpp"

Request* RequestParser::parse(char* data) {
	Request* request = new Request();
	std::istringstream iss(data);

	std::string line;
	getline(iss, line);
	std::istringstream issLine(line);

	parseStartLine(issLine, request);

	parseHeaders(iss, request);
	return request;
}

bool RequestParser::parseStartLine(std::istringstream& line, Request* request) {
	std::string token;

	line >> token;
	parseMethod(request, token);

	line >> token;
	parseUri(request, token);

	line >> token;
	return parseHttpVersion(request, token);
}

void RequestParser::parseMethod(Request* request, const std::string& method) const {
	if (method == "GET")
		request->setMethod(GET);
	else if (method == "PUT")
		request->setMethod(PUT);
	else if (method == "POST")
		request->setMethod(POST);
	else if (method == "DELETE")
		request->setMethod(DELETE);
	else {
//		request->setMethod(UNKNOWN_METHOD);
		request->setBadStatus();
//		return false;
	}
}

void RequestParser::parseUri(Request* request, const std::string& uri) const {
	if ((uri[0] == '/') || (uri == "*")) {
		request->setUri(uri);
	}
	else {
		request->setBadStatus();
//		return false;
	}
}

bool RequestParser::parseHttpVersion(Request* request, const std::string& httpVersion) const {
	if (httpVersion == "HTTP/1.0" || httpVersion == "HTTP/1.1")
		request->setHttpVersion(httpVersion);
	else {
		request->setBadStatus();
		return false;
	}
	return true;
}

void RequestParser::parseHeaders(std::istringstream& iss, Request* request) {
	std::string line;
	std::map<std::string, std::string> headers;

	while (getline(iss, line)) {
		int separator = line.find(':');
		std::string header = line.substr(0, separator);
		std::string value = line.substr(separator + 2);
		headers[header] = value;
	}
	request->setHeaders(headers);

	std::map<std::string, std::string>::iterator it = headers.find("Host");
	if (it == headers.end()) {
		request->setBadStatus();
		return;
	}
}

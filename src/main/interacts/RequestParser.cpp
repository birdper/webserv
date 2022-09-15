#include "RequestParser.hpp"

Request* RequestParser::parse(const string& requestString) {
    Request* request = new Request();

    if (requestString.find(HTTP_BORDER_LINE) == string::npos) {
        return nullptr;
    }

    std::istringstream iss(requestString);
    string startLine;
    getline(iss, startLine);

    try {
        parseStartLine(startLine, *request);
        parseHeaders(iss, *request);
        parseBody(*request);
    } catch (ParseRequestException& ex) {
        request->setBadStatus();
    }
    return request;
}

void RequestParser::parseStartLine(const string& line, Request& request) {
    std::vector<string> startLineTokens = Utils::split(line, " ");

    if (startLineTokens.size() != 3) {
        throw ParseRequestException("Incorrect format Start Line '" + line + "'");
    }

    parseMethod(request, startLineTokens[0]);
    parseUri(request, startLineTokens[1]);
    parseHttpVersion(request, startLineTokens[2]);
}

void RequestParser::parseMethod(Request& request, const string& httpMethod) const {
    request.setHttpMethodString(httpMethod);
    if (httpMethod == "GET") {
        request.setMethod(GET);
    } else if (httpMethod == "PUT") {
        request.setMethod(PUT);
    } else if (httpMethod == "POST") {
        request.setMethod(POST);
    } else if (httpMethod == "DELETE") {
        request.setMethod(DELETE);
    } else {
        throw ParseRequestException("Wrong HTTP Method '" + httpMethod + "'");
    }
}

void RequestParser::parseUri(Request& request, const string& uri) const {
    if ((uri[0] == '/')) {
        request.setUri(uri);
    } else {
        throw ParseRequestException("Incorrect uri '" + uri + "'");
    }
}

void RequestParser::parseHttpVersion(Request& request, const string& httpVersion) const {
    if (httpVersion == "HTTP/1.0" || httpVersion == "HTTP/1.1") {
        request.setHttpVersion(httpVersion);
    } else {
        throw ParseRequestException("Incorrect Version Protocol '" + httpVersion + "'");
    }
}

void RequestParser::parseHeaders(std::istringstream& iss, Request& request) {
    string line;
    std::map<string, string> headers;

    while (getline(iss, line)) {
        int separator = line.find(':');
        string header = line.substr(0, separator);
        string value = line.substr(separator + 2);
        headers[header] = value;
    }
    request.setHeaders(headers);

    std::map<string, string>::iterator it = headers.find("Host");
    if (it == headers.end()) {
        throw ParseRequestException("Not found header 'Host'");
    }
}

void RequestParser::parseBody(Request& request) {
//	TODO (not yet implement)
    if ((request.getHttpMethod() != POST) && (request.getHttpMethod() != PUT)) {
        return;
    }

}

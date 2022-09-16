#include "RequestParser.hpp"

Request* RequestParser::parse(const string& requestString) {
    Request* request = new Request();

    size_t borderLinePosition = requestString.find(HTTP_BORDER_LINE);
    if (borderLinePosition == string::npos) {
        std::cout << "not found borderline!" << std::endl;
        return nullptr;
    }

//    if (pos != string::npos && request->emptyHeader()) {
    string headersString = requestString.substr(0, borderLinePosition);
    string bodyString(requestString.substr(borderLinePosition + 4));

    std::vector<string> headerLines = Utils::split(headersString, END_OF_LINE);
//}

    try {
        parseStartLine(headerLines[0], *request);
        parseHeaders(headerLines, *request);
        parseBody(*request);
    } catch (ParseRequestException& ex) {
        Utils::printStatus(ex.what());
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
    request.setMethodString(httpMethod);
    if (httpMethod == "GET") {
        request.setMethodEnum(GET);
    } else if (httpMethod == "PUT") {
        request.setMethodEnum(PUT);
    } else if (httpMethod == "POST") {
        request.setMethodEnum(POST);
    } else if (httpMethod == "DELETE") {
        request.setMethodEnum(DELETE);
    } else {
        throw ParseRequestException("Wrong HTTP Method '" + httpMethod + "'");
    }
}

void RequestParser::parseUri(Request& request, const string& uri) const {
    if (uri[0] == '/') {
        request.setUri(uri);
    } else {
        throw ParseRequestException("Incorrect uri '" + uri + "'");
    }
}

void RequestParser::parseHttpVersion(Request& request, const string& httpVersion) const {
    if (httpVersion == "HTTP/1.1") {
        request.setHttpVersion(httpVersion);
    } else {
        throw ParseRequestException("Incorrect Version Protocol '" + httpVersion + "'");
    }
}

void RequestParser::parseHeaders(std::vector<string>& headerLines, Request& request) {
    std::map<string, string>* headers = new std::map<string, string>();

    for (int i = 1; i < headerLines.size(); ++i) {
        size_t separatorPosition = headerLines[i].find(':');
        string header = headerLines[i].substr(0, separatorPosition);
        string value = headerLines[i].substr(separatorPosition + 2);
        (*headers)[header] = value;
    }
    request.setHeaders(*headers);

    if (request.findHeaderValue("Host").empty()) {
        throw ParseRequestException("Not found header 'Host'");
    }
//    std::map<string, string>::iterator it = headers->find("Host");
//    if (it == headers->end()) {
//        throw ParseRequestException("Not found header 'Host'");
//    }
}

void RequestParser::parseBody(Request& request) {
//	TODO (not yet implement)
    if ((request.getHttpMethod() != POST) && (request.getHttpMethod() != PUT)) {
        return;
    }
    string contentLength = request.findHeaderValue("Content-Length");

}

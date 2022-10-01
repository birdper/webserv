#include "RequestParser.hpp"
#include "Client.hpp"

void RequestParser::parse(const string& requestBuffer, Request& request, Client& client) const {

    Utils::printStatus("parse request");
    std::cout << "=========REQUEST_BUFFER=========\n" << requestBuffer << "=========END REQUEST=========" << std::endl;
    size_t borderLinePosition = requestBuffer.find(HTTP_BORDER_LINE);


    if (borderLinePosition != string::npos && request.getHeaders().empty()) {
        Utils::printStatus("parse headers");
        string headersString = requestBuffer.substr(0, borderLinePosition);

        string bodyString(requestBuffer.substr(borderLinePosition + HTTP_BORDER_LINE.length()));
        request.setBody(bodyString);

        std::vector<string> headerLines = Utils::split(headersString, END_OF_LINE);

        parseStartLine(headerLines[0], request);
        parseHeaders(headerLines, request);

        if (!(request.getHttpMethod() == POST || request.getHttpMethod() == PUT)) {
            Utils::printStatus("REQUEST READY");
            client.setIsReadyRequest(true);
			client.setBuffer("");
        }
//        parsePost(request);
    }
//    try {
    if (request.getHttpMethod() == POST || request.getHttpMethod() == PUT) {
        parseBody(request, requestBuffer, client);
    }
//    } catch (ParseRequestException& ex) {
//        Utils::printStatus(ex.what());
//        request.setBadStatus();
//    }
//    return request;
}

bool RequestParser::isHasHeaders(const Request& request) const {
    return request.getHeaders().empty();
}

void RequestParser::parseStartLine(const string& line, Request& request) const {
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

void RequestParser::parseHeaders(std::vector<string>& headerLines, Request& request) const {
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

void RequestParser::parsePostHeaders(Request& request) const {
    if (request.getHttpMethod() == POST || request.getHttpMethod() == PUT) {
        if (request.findHeaderValue("Transfer-Encoding") == "chunked") {
//            request.setPostType(CHUNKED);
            return;
        }
        string contentLength = request.findHeaderValue("Content-Length");
        if (!contentLength.empty()) {
            ContentLengthToInt(contentLength);
//            request.setPostType(SIMPLE)
            return;
        }
    }
}


long RequestParser::ContentLengthToInt(const string& contentLength) const {

    char* endPtr;
    long number = std::strtol(contentLength.c_str(), &endPtr, 10);

    if (number < INT32_MIN || number > INT32_MAX) {
        throw ParseRequestException("Content-Length value incorrect number");
    }
    return number;
}

bool RequestParser::parseBody(Request& request, const string& clientBuffer, Client& client) const {

    if (request.findHeaderValue("Transfer-Encoding") == "chunked") {
        return parseChunked(request);
    }

    string contentLength = request.findHeaderValue("Content-Length");
    if (!contentLength.empty()) {
        parseBodyContent(request, clientBuffer, client);
    }
    return false;
}


void RequestParser::parseBodyContent(Request& request, const string& clientBuffer, Client& client) const {
//    TODO перенести в PostHandler
//    string fileName = Utils::getFileName(request.getLocationUri());
//    request.setFileName(fileName);
    Utils::printStatus("parse content");

    size_t contentLength = ContentLengthToInt(request.findHeaderValue("Content-Length"));
    if (request.getBody().size() > contentLength) {
        throw ParseRequestException("400 BAD REQUEST! RECV size > then MUST BE");
    }
    request.appendBody(clientBuffer);
    if (contentLength == request.getBody().size()) {
        Utils::printStatus("POST REQUEST READY");
        client.setIsReadyRequest(true);
	    client.setBuffer("");
    }
}

bool RequestParser::parseChunked(Request& request) const {
    Utils::printStatus("parse chunked");
    return true;

//    TODO Not yet implement
}

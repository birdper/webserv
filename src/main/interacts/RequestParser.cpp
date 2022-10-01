#include "RequestParser.hpp"
#include "Client.hpp"

void RequestParser::parse(const string& requestBuffer, Request& request, Client& client) const {

//    std::cout << "=========REQUEST_BUFFER=========\n" << requestBuffer << "=========END REQUEST=========" << std::endl;
    size_t borderLinePosition = requestBuffer.find(HTTP_BORDER_LINE);

    string bodyString;
    if (borderLinePosition != string::npos && isHasHeaders(request)) {
        Utils::printStatus("parse headers");
        string headersString = requestBuffer.substr(0, borderLinePosition);
        std::vector<string> headerLines = Utils::split(headersString, END_OF_LINE);

//        bodyString = requestBuffer.substr(borderLinePosition + HTTP_BORDER_LINE.length());
//        request.setBody(bodyString);
        request.setBuffer(requestBuffer.substr(borderLinePosition + HTTP_BORDER_LINE.length()));

        parseStartLine(headerLines[0], request);
        parseHeaders(headerLines, request);

//        TODO delete debug print
        printRequest(headerLines);

//        if (request.getHttpMethod() != POST && request.getHttpMethod() != PUT) {
//            Utils::printStatus("REQUEST READY");
//            client.setIsReadyRequest(true);
//            client.setBuffer("");
//        }
//        parsePost(request);
    }
//    try {
    if (!request.getMethodString().empty()) {
        string contentLength = request.findHeaderValue("Content-Length");
        if (request.getHttpMethod() != POST && request.getHttpMethod() != PUT) {
            Utils::printStatus("REQUEST READY");
            client.setIsReadyRequest(true);
            client.setBuffer("");
        } else
//    if (request.getHttpMethod() == POST || request.getHttpMethod() == PUT) {
//        parseBody(request, requestBuffer, client);
        if (request.findHeaderValue("Transfer-Encoding") == "chunked") {
            parseChunked(request, client);
//            return true;
        } else if (!contentLength.empty()) {
            parseBodyContent(request, bodyString, client);
        }
    }
//        return false;
    std::cout << std::endl;

}

//    } catch (ParseRequestException& ex) {
//        Utils::printStatus(ex.what());
//        request.setBadStatus();
//    }
//    return request;

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
        parseChunked(request, client);
        return true;
    }

    string contentLength = request.findHeaderValue("Content-Length");
    if (!contentLength.empty()) {
        parseBodyContent(request, clientBuffer, client);
    }
    return false;
}


void RequestParser::parseBodyContent(Request& request, const string& buffer, Client& client) const {
//    TODO перенести в PostHandler
//    string fileName = Utils::getFileName(request.getLocationUri());
//    request.setFileName(fileName);
    Utils::printStatus("parseBodyContent");

    size_t contentLength = ContentLengthToInt(request.findHeaderValue("Content-Length"));
    std::cout << "request.getBody().size() " << request.getBody().size() << std::endl;
    std::cout << "contentLength " << contentLength << std::endl;
//    if (request.getBody().size() > contentLength) {
//        throw ParseRequestException("400 BAD REQUEST! RECV size > then MUST BE");
//    }
//    request.appendBody(buffer);

    string body = request.getBuffer();
    request.setBuffer("");
    //	Добавляют в боди новую порцию из буффера
    request.setBody(request.getBody() + body);

    if (contentLength == request.getBody().size()) {
        Utils::printStatus("BODY CONTENT DONE");
        client.setIsReadyRequest(true);
        client.setBuffer("");
    }
}

void RequestParser::parseChunked(Request& request, Client& client) const {
    const size_t newLineLen = END_OF_LINE.length();
    const string END_OF_CHUNKED_REQUEST = "0" + HTTP_BORDER_LINE;

    const string& requestBuffer = client.getBuffer();
    size_t chunkedRequestEndPos = requestBuffer.find(END_OF_CHUNKED_REQUEST);

    if (chunkedRequestEndPos != std::string::npos) {
        if (requestBuffer.length() != chunkedRequestEndPos + END_OF_CHUNKED_REQUEST.length()) {
            throw ParseRequestException("");
        }
        size_t chunkEndPos = 0;
        while (chunkEndPos < chunkedRequestEndPos) {
            size_t chunkStartPos = requestBuffer.find(END_OF_LINE, chunkEndPos);
            size_t chunkLength = Utils::stringToInt(requestBuffer.substr(chunkEndPos, chunkStartPos - chunkEndPos), 16);
            chunkStartPos += newLineLen;

            chunkEndPos = requestBuffer.find(END_OF_LINE, chunkStartPos);
            string chunk = requestBuffer.substr(chunkStartPos, chunkEndPos - chunkStartPos);
            chunkEndPos += newLineLen;

            if (chunk.length() != chunkLength) {
                throw ParseRequestException("");
            }
            request.setBody(request.getBody() + chunk);
        }
        client.setBuffer("");
        Utils::getFileName(request.getUri());
        client.setIsReadyRequest(true);
    }
}

void RequestParser::printRequest(const std::vector<string>& headerLines) const {
    cout << "\n=========REQUEST=========" << endl;
    for (int i = 0; i < headerLines.size(); ++i) {
        std::cout << headerLines[i] << std::endl;
    }
    cout << "=========END_REQUEST============" << endl << endl;
}
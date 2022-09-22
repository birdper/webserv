//
// Created by Aleksei S on 06.09.2022.
//

#include "Response.hpp"

std::string& Response::serialize() {
    std::ostringstream ss;

    // start-line
    ss << "HTTP/1.1 " << statusCode << END_OF_LINE;

	string& headers = parseToHeaders();
    ss << headers;
    ss << "Date: " << Utils::getCurrentTimestamp(false, "%a, %d %b %Y %H:%M:%S GMT") << END_OF_LINE;

    if (!body.empty()) {
		ss << END_OF_LINE;
        ss << body << END_OF_LINE;
    }

    ss << END_OF_LINE;
    return *new std::string(ss.str());
}

const std::string& Response::getReason() const {
    return reason;
}

void Response::setReason(const std::string& reason) {
    this->reason = reason;
}

const std::string& Response::getStatusCode() const {
    return statusCode;
}

void Response::setStatusCode(const std::string& statusCode) {
    this->statusCode = statusCode;
}

const std::string& Response::getBody() const {
    return body;
}

void Response::setBody(const std::string& body) {
    Response::body = body;
}

string& Response::parseToHeaders() {
	std::ostringstream ss;
	std::map<string, string> headers = getHeaders();
	std::map<string, string>::iterator it = headers.begin();
	for (; it != headers.end(); ++it) {
		ss << it->first << ": " << it->second << END_OF_LINE;
	}
	return *new string (ss.str());
}

Response::~Response() {
}
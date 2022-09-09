//
// Created by Aleksei S on 06.09.2022.
//

#include "Response.hpp"

std::string& Response::serialize() {
	std::stringstream ss;

	// start-line
	ss << version << ' '
	   << statusCode << "\r\n";

	// date header
	ss << "Date: " << Utils::getTimestamp("%a, %d %b %Y %H:%M:%S GMT", false) << "\r\n";

	return *new std::string (ss.str());
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

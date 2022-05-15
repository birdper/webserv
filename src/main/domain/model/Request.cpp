#include "Request.hpp"

bool Request::isBadRequest() const {
	return isBadStatus;
}

void Request::setBadStatus() {
	Request::isBadStatus = true;
}

HttpMethod Request::getMethod() const {
	return method;
}

void Request::setMethod(HttpMethod method) {
	Request::method = method;
}

const std::string& Request::getUri() const {
	return uri;
}

void Request::setUri(const std::string& requestUri) {
	Request::uri = requestUri;
}

const std::string& Request::getHttpVersion() const {
	return httpVersion;
}

void Request::setHttpVersion(const std::string& version) {
	httpVersion = version;
}

const std::map<std::string, std::string>& Request::getHeaders() const {
	return headers;
}

void Request::setHeaders(const std::map<std::string, std::string>& headers) {
	Request::headers = headers;
}

Request::Request(const Request& other) :
		isBadStatus(other.isBadStatus),
		method(other.method),
		uri(other.uri),
		httpVersion(other.httpVersion) {
}

Request& Request::operator=(const Request&) {
	return *this;
}

Request::Request() {

}

const std::string& Request::findHeaderByName(const std::string& header) const {
	std::map<std::string, std::string>::const_iterator it = headers.find(header);
	if (it == headers.end())
		return *new std::string("");
	return it->second;
}





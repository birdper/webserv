#include "Request.hpp"

Request::Request() {
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

Request::~Request() {

}

bool Request::isBadRequest() const {
	return isBadStatus;
}

void Request::setBadStatus() {
	Request::isBadStatus = true;
}

HttpMethod Request::getHttpMethod() const {
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

const std::string& Request::findHeaderByName(const std::string& headerName) const {
	std::map<std::string, std::string>::const_iterator it = headers.find(headerName);
	if (it == headers.end())
		return *new std::string("");
	return it->second;
}

const std::string& Request::getHttpMethodString() const {
    return httpMethodString;
}

void Request::setHttpMethodString(const std::string& httpMethodString) {
    this->httpMethodString = httpMethodString;
}





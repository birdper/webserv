#include "Request.hpp"

Request::Request() {
    isBadStatus = false;
}

Request::Request(const Request& other) :
        _methodEnum(other._methodEnum),
        _methodString(other._methodString),
        uri(other.uri),
        isBadStatus(other.isBadStatus) {
    _httpVersion = other._httpVersion;
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
	return _methodEnum;
}

void Request::setMethodEnum(HttpMethod method) {
	Request::_methodEnum = method;
}

const std::string& Request::getUri() const {
	return uri;
}

void Request::setUri(const std::string& requestUri) {
	Request::uri = requestUri;
}

const std::string& Request::getMethodString() const {
    return _methodString;
}

void Request::setMethodString(const std::string& httpMethodString) {
    this->_methodString = httpMethodString;
}

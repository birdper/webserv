#include "Client.hpp"

Client::Client(int socketDescriptor) :
        socketDescriptor(socketDescriptor) {
    response = nullptr;
    _isReadyRequest = false;
}

Client::Client(int socketDescriptor, int listenSocketDescriptor) :
        socketDescriptor(socketDescriptor),
        listenSocketDescriptor(listenSocketDescriptor) {
    response = nullptr;
    _isReadyRequest = false;
}

Client::~Client() {
//	TODO из-за этих методов сега
//	clear();
}

int Client::getSocketDescriptor() const {
    return socketDescriptor;
}

const std::string& Client::getBuffer() const {
    return buffer;
}

void Client::setBuffer(const std::string& buffer) {
    this->buffer = buffer;
}

void Client::clear() {
//    delete (request);
    delete (response);
}

int Client::getListenSocketDescriptor() const {
    return listenSocketDescriptor;
}

bool Client::isReadyRequest() const {
    return _isReadyRequest;
}

void Client::setIsReadyRequest(bool isReadyRequest) {
    Client::_isReadyRequest = isReadyRequest;
}

Response* Client::getResponse() const {
    return response;
}

void Client::setResponse(Response* response) {
    Client::response = response;
}

void Client::appendToBuffer(const string& chunk) {
    this->buffer.append(chunk);
}

Request& Client::getRequest() {
    return request;
}

const string& Client::getCookieId() const {
    return cookieId;
}

void Client::setCookieId(const string& cookieId) {
    Client::cookieId = cookieId;
}

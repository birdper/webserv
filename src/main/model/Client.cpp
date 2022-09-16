#include "Client.hpp"

Client::Client(int socketDescriptor) :
        socketDescriptor(socketDescriptor) {
}

Client::Client(int socketDescriptor, int listenSocketDescriptor) :
        socketDescriptor(socketDescriptor),
        listenSocketDescriptor(listenSocketDescriptor) {
}

Client::~Client() {
//	TODO из-за этих методов сега
//	clear();
//    clearSendQueue();
}

void Client::addToSendQueue(std::string* item) {
    sendQueue.push(item);
}

unsigned int Client::sendQueueSize() {
    return sendQueue.size();
}

std::string* Client::nextInSendQueue() {
    if (sendQueue.empty())
        return nullptr;

    return sendQueue.front();
}

void Client::dequeueFromSendQueue() {
    std::string* item = nextInSendQueue();
    if (item != nullptr) {
        sendQueue.pop();
        delete item;
    }
}

void Client::clearSendQueue() {
    while (!sendQueue.empty()) {
        delete sendQueue.front();
        sendQueue.pop();
    }
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
    delete (request);
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

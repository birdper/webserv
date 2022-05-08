#include "Client.hpp"

Client::Client(int socketDescriptor, const sockaddr_in& clientAddress) :
		socketDescriptor(socketDescriptor),
		clientAddress(clientAddress) {}

Client::~Client() {
	clearSendQueue();
}

void Client::refresh() {
/*	delete request;
	delete response;
	request = new Request();
	response = new Response();
 */
	setStatus(Readable);
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

Client::Status Client::getStatus() const {
	return status;
}

void Client::setStatus(Status status) {
	this->status = status;
}

int Client::getSocketDescriptor() const {
	return socketDescriptor;
}

//void WebClient::setSocketDescriptor(int socketDescriptor) {
//	WebClient::socketDescriptor = socketDescriptor;
//}

const std::string& Client::getBuffer() const {
	return buffer;
}

void Client::setBuffer(const std::string& buffer) {
	Client::buffer = buffer;
}
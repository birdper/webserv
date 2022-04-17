
#include "WebClient.hpp"

WebClient::WebClient(int socketDescriptor, const sockaddr_in& clientAddress) :
		socketDescriptor(socketDescriptor),
		clientAddress(clientAddress) {}

WebClient::~WebClient() {
	clearSendQueue();
}

void WebClient::refresh() {
/*	delete request;
	delete response;
	request = new Request();
	response = new Response();
 */
	setStatus(Readable);
}

void WebClient::addToSendQueue(std::string * item) {
	sendQueue.push(item);
}

unsigned int WebClient::sendQueueSize() {
	return sendQueue.size();
}

std::string* WebClient::nextInSendQueue() {
	if (sendQueue.empty())
		return nullptr;

	return sendQueue.front();
}

void WebClient::dequeueFromSendQueue() {
	std::string* item = nextInSendQueue();
	if (item != nullptr) {
		sendQueue.pop();
		delete item;
	}
}

void WebClient::clearSendQueue() {
	while (!sendQueue.empty()) {
		delete sendQueue.front();
		sendQueue.pop();
	}
}

WebClient::Status WebClient::getStatus() const {
	return status;
}

void WebClient::setStatus(Status status) {
	this->status = status;
}

int WebClient::getSocketDescriptor() const {
	return socketDescriptor;
}

//void WebClient::setSocketDescriptor(int socketDescriptor) {
//	WebClient::socketDescriptor = socketDescriptor;
//}

const std::string& WebClient::getBuffer() const {
	return buffer;
}

void WebClient::setBuffer(const std::string& buffer) {
	WebClient::buffer = buffer;
}
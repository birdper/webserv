#include "Client.hpp"


/*
Client::Client(int socketDescriptor,
			   int* listenSocketDescriptor,
			   const sockaddr_in& clientAddress) :
		socketDescriptor(socketDescriptor),
		listenSocketDescriptor(listenSocketDescriptor),
		clientAddress(clientAddress) {}
*/

Client::Client(int socketDescriptor) : socketDescriptor(socketDescriptor) {}

Client::~Client() {
    clearSendQueue();
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
	Client::buffer = buffer;
}

/*int Client::getHostPort() const {
	sockaddr_in address;
	getsockname(*listenSocketDescriptor,
				(sockaddr*)&address,
				(socklen_t*)sizeof(address));
	return address.sin_port;
}*/

#pragma once

#include <iostream>
#include <netinet/in.h>
#include <queue>
#include "Response.hpp"
#include "Request.hpp"

class Client {
public:
	enum Status {
		Readable,
		Writing,
		Close
	};

private:
	int socketDescriptor;
	int *listenSocketDescriptor;
//	TODO clientAddress necessary?
	sockaddr_in clientAddress;
	Status status;

	Request* request;
	Response* response;
	std::string buffer;

	std::queue<std::string*> sendQueue;

public:
	Client(int socketDescriptor, int* listenSocketDescriptor, const sockaddr_in& clientAddress);

	virtual ~Client();

	void refresh(); // TODO ?

	void addToSendQueue(std::string* item);
	unsigned int sendQueueSize();
	std::string* nextInSendQueue();
	void dequeueFromSendQueue();
	void clearSendQueue();
	int getSocketDescriptor() const;
//	void setSocketDescriptor(int socketDescriptor);
	Status getStatus() const;
	void setStatus(Status status);
	const std::string& getBuffer() const;
	void setBuffer(const std::string& buffer);
	int getHostPort() const;
};
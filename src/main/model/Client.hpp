#pragma once

#include <iostream>
#include <netinet/in.h>
#include <queue>
#include "Response.hpp"
#include "Request.hpp"

class Client {
private:
	int socketDescriptor;

	Request* request;
	Response* response;
	std::string buffer;

	std::queue<std::string*> sendQueue;

public:
	/*Client(int socketDescriptor, int* listenSocketDescriptor, const sockaddr_in& clientAddress)*/;

    Client(int socketDescriptor);

    virtual ~Client();

	void addToSendQueue(std::string* item);
	unsigned int sendQueueSize();
	std::string* nextInSendQueue();
	void dequeueFromSendQueue();
	void clearSendQueue();
	int getSocketDescriptor() const;

    const std::string& getBuffer() const;
	void setBuffer(const std::string& buffer);
    void clear();
	/*int getHostPort() const;*/
};
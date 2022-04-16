#pragma once

#include <iostream>
#include <netinet/in.h>
#include <queue>

class WebClient {
public:
	enum Status {
		Readable,
		Writing,
		Close
	};

private:
	int socketDescriptor;
	sockaddr_in clientAddress;
	Status status;

	std::string buffer;

	std::queue<std::string*> sendQueue;

public:
	WebClient(int socketDescriptor, const sockaddr_in& clientAddress);
	virtual ~WebClient();

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
};
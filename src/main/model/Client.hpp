#pragma once

#include <iostream>
#include <netinet/in.h>
#include <queue>
#include "usings.hpp"
#include "Response.hpp"
#include "Request.hpp"

class Client {
private:
    int socketDescriptor;
    int listenSocketDescriptor;

	Request* request;
	Response* response;

	std::string buffer;
    std::string cookie;
	std::queue<std::string*> sendQueue;

    bool _isReadyRequest;

public:
    bool isReadyRequest() const;
    void setIsReadyRequest(bool isReadyRequest);

    explicit Client(int socketDescriptor);
    Response* getResponse() const;
    void setResponse(Response* response);
    Client(int socketDescriptor, int listenSocketDescriptor);
    virtual ~Client();

	int getSocketDescriptor() const;
    int getListenSocketDescriptor() const;

    void addToSendQueue(std::string* item);
	unsigned int sendQueueSize();
    std::string* nextInSendQueue();
	void dequeueFromSendQueue();
	void clearSendQueue();

    const std::string& getBuffer() const;
	void setBuffer(const std::string& buffer);

    void clear();
};
#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include "Constants.hpp"

class Socket {
private:
	int socketDescriptor;

public:
	int initSocket(int listenPort, in_addr_t ip = INADDR_ANY);

private:

	void createSocket();
	void checkError(int result, const std::string& nameFunFromError);
	void setNonblockMode();
	void setAddressReuseMode();
	struct sockaddr_in initAddress(int listenPort, in_addr_t ip);
	void bindToAddress(sockaddr_in address);
	void startListening();
};
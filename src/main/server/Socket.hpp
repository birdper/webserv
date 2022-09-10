#pragma once

#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>

#include "Constants.hpp"

class Socket {
private:
	int socketDescriptor;

public:
	int init(const std::string &ip, int listenPort);

private:
	void openSocket();
	void checkError(int result, const std::string& nameFunFromError);
	void setNonblockMode();
	void setAddressReuseMode();
	struct sockaddr_in initAddress(int listenPort, in_addr_t ip);
	void bindToAddress(sockaddr_in address);
	void startListening();
};
#pragma once

#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include "usings.hpp"
#include "Constants.hpp"

class Socket {
private:
	int socketDescriptor;
    string ip;
    int port;

public:
    Socket(const string& ip, int port);

    int getSocketDescriptor() const;
    const string& getIp() const;
    string getPort() const;

private:
    void init();
	void openSocket();
	void checkError(int result, const std::string& nameFunFromError);
	void setNonblockMode();
	void setAddressReuseMode();
	struct sockaddr_in initAddress();
	void bindToAddress(sockaddr_in address);
	void startListening();
};
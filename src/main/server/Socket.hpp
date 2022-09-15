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
	int _socketDescriptor;
    string _ip;
    int _port;

public:
    Socket(const Socket& other);
    Socket& operator=(Socket other);

    int getSocketDescriptor() const;
    const string& getIp() const;
    string getPortString() const;

	static void setNonblockMode(int socketDescriptor);
	static Socket* getInstance(const string& ip, int port);


private:
    Socket(const string& ip, int port);
    void init();
	void openSocket();
	static void checkError(int result, const std::string& nameFunFromError);
	void setAddressReuseMode();
	struct sockaddr_in initAddress();
	void bindToAddress(sockaddr_in address);
	void startListening();
};
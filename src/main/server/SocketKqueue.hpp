#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>

#define INVALID_SOCKET -1

class SocketKqueue {
//	int listenPort;
//	in_addr_t mIp;

	int listenSocketDescriptor; // Descriptor for the listening socket
	struct sockaddr_in serverAddr; // Structure for the server address

//	int dropUid; // setuid to this after bind()
//	int dropGid; // setgid to this after bind()

public:
	int init(int listenPort, in_addr_t ip);

private:
	void initAddress(int listenPort, in_addr_t ip);
};
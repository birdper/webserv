#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>



class Socket {
	/** Server Socket */
	int listenPort;
	int listenSocketDescriptor; // Descriptor for the listening socket
	struct sockaddr_in serverAddr; // Structure for the server address
//	int dropUid; // setuid to this after bind()
//	int dropGid; // setgid to this after bind()
public:
	bool start();
};
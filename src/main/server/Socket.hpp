#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include "Constants.hpp"


class Socket {

public:
	int initSocket(int listenPort, in_addr_t ip = INADDR_ANY);

private:
	struct sockaddr_in initAddress(int listenPort, in_addr_t ip);
};
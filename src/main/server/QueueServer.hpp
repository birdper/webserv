//
// Created by a19665881 on 21.03.2022.
//

#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

class QueueServer {
private:
//	static const int QUEUE_SIZE = 1024;
//	static const int INVALID_SOCKET = -1;
	bool canRun;
	int listenSocketDescriptor;


public:
	bool start();
	void process();
	void stop();
};



#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>
#include <poll.h>


class HTTPServer {
private:
	std::vector<struct pollfd> fds;

	// ?
	static const int timeout = -1;

public:
	void process();

	bool isNothingEvents(int clientIndex);
};
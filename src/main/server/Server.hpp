#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <unistd.h>

#include "Constants.hpp"
#include "Socket.hpp"
#include "WebClient.hpp"
#include "RequestHandler.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"

class Server {

private:
//	std::vector<int> openSockets;
	RequestParser& requestParser;
	RequestHandler& requestHandler;

	std::vector<struct pollfd> events;
	std::vector<WebClient*> clients;

	nfds_t numberEvents;
	nfds_t currentItem;

public:
	Server(RequestParser& requestParser,
		   RequestHandler& requestHandler);

	void initSockets(std::vector<int>& hosts);
	void mainLoop();

private:
	void polling();
	void acceptConnections();
	void closeConnections();
	bool receiveRequest();
	bool sendResponse(Response* response);

	struct pollfd getPollFd(int socketDescriptor, short events);
	WebClient* getClient() const;

	bool hasEvents() const;
	bool isReadable() const;
	bool isWriteable() const;
	void nextClient();
	bool hasReadyClient();

};


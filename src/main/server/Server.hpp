#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <unistd.h>

#include "Constants.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "RequestHandler.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"
#include "ClientRepository.hpp"

class Server {

private:
	typedef unsigned char byte;

	RequestParser& requestParser;
	RequestHandler& requestHandler;

	int countListenSockets;
	std::vector<struct pollfd> pollFds;

	ClientRepository clientRepository;

public:
	Server(RequestParser& requestParser,
		   RequestHandler& requestHandler);

	void initSockets(std::vector<int>& hosts);
	void mainLoop();

private:
	struct pollfd initPollFd(int socketDescriptor, short eventTypes);

	void polling();
	void acceptConnections();

	void handleEvents();

	bool readClient(Client* client);
	bool writeClient(Client* client, Response* response);

	void disconnectClient(Client* client, bool isRemoveClient);
	void closeConnections();

	void handleRequest(Client* client, Request* request);
};


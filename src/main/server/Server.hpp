#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>

#include "Constants.hpp"
#include "Socket.hpp"
#include "RequestHandler.hpp"
#include "RequestParser.hpp"
#include "ClientRepository.hpp"
#include "ConfigRepository.hpp"
#include "Client.hpp"
#include "Response.hpp"

class Server {

private:
	RequestParser& requestParser;
	RequestHandler& requestHandler;
	ConfigRepository& configRepository;
	ClientRepository clientRepository;

	int countListenSockets;
	std::vector<struct pollfd> pollFds;
	static const int BUFFER_SIZE = 1024;

public:
	Server(RequestParser& requestParser,
		   RequestHandler& requestHandler,
		   ConfigRepository& settingsRepository);

	void initSockets();
	void run();

private:
	struct pollfd initPollFd(int socketDescriptor, short eventTypes);

	void polling();
	void acceptClient(pollfd& listenSocket);

	void handleEvents();

	bool readClient(Client* client);
	bool writeClient(Client* client);

	void disconnectClient(Client* client, bool isRemoveClient);
	void closeConnections();
};


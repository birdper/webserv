#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>

#include "usings.hpp"
#include "Constants.hpp"
#include "Socket.hpp"
#include "RequestHandler.hpp"
#include "RequestParser.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "VirtualServer.hpp"
#include "ClientRepository.hpp"
#include "ConfigRepository.hpp"


class Server {

private:
	RequestParser& requestParser;
	ConfigRepository& configRepository;
	ClientRepository clientRepository;

	int countListenSockets;
	std::vector<struct pollfd> pollFds;
	static const int BUFFER_SIZE = 1024;
    std::map<int, Socket*> listenSockets;

public:
	Server(RequestParser& requestParser,
		   ConfigRepository& settingsRepository);

	void initSockets();
	void run();

private:
    void putListenSocket(Socket& socket);

	struct pollfd initPollFd(int socketDescriptor, short eventTypes);

	void polling();
	void acceptClient(pollfd& listenSocket);

	void handleEvents();

	short readClient(Client* client);
	short writeClient(Client* client);

	void disconnectClient(Client* client, bool isRemoveClient);
    Config& findConfig(const Client& client, const Request& request);
};


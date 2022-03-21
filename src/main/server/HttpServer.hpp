
#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <unistd.h>
#include <poll.h>

#include "SimpleServer.hpp"
#include "Client.hpp"


class HttpServer {

private:
	std::vector<SimpleServer*> servers;
	std::vector<struct pollfd> pollfds;
	std::vector<Client*> clients;
	nfds_t numberOfListeners;
	nfds_t currentItem;

public:
	explicit HttpServer(std::vector<std::string>&);
	~HttpServer();

	void polling();
	void createConnections();
	void closeConnections();
	bool receiveRequest();
	bool toSendResponse();
	Client *getClient() const;

	void nextClient();
	bool haveRawClients();



private:
	HttpServer();
	HttpServer(const HttpServer &other);
	HttpServer &operator=(const HttpServer &other);

	bool isHasEvents() const;
	bool isReadable() const;
	bool isWriteable() const;
};

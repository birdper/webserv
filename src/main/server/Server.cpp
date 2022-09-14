

#include "Server.hpp"

Server::Server(RequestParser& requestParser,
			   RequestHandler& requestHandler,
			   ConfigRepository& settingsRepository) :
		requestParser(requestParser),
		requestHandler(requestHandler),
		configRepository(settingsRepository) {}

void Server::initSockets() {
	Socket socket;

	std::vector<std::pair<string, int> > hosts = configRepository.getHostsForBind();

	for (size_t i = 0; i < hosts.size(); ++i) {
		string& ip = hosts[i].first;
		int port = hosts[i].second;
		int socketDescriptor = socket.init(ip, port);
		pollFds.push_back(initPollFd(socketDescriptor, POLLIN));

		printStatus("Listening socket start on " + ip + ":" + std::to_string(port));
	}
	countListenSockets = hosts.size();
}

struct pollfd Server::initPollFd(int socketDescriptor, short eventTypes) {
	struct pollfd pollFd;
	pollFd.fd = socketDescriptor;
	pollFd.events = eventTypes;
	pollFd.revents = NoEvents;
	return pollFd;
}

#pragma clang diagnostic push

#pragma ide diagnostic ignored "EndlessLoop"

void Server::run() {
	initSockets();

	while (true) {
		polling();
		handleEvents();
	}
}

#pragma clang diagnostic pop

void Server::polling() {
	int result = poll(pollFds.data(), pollFds.size(), InfinityTimeout);
//	TODO read man, what handling errors
	if (result < 0) {
		cerr << "poll() failed: " << strerror(errno) << endl;
	} else if (result == 0) {
		cerr << "poll() timed out. End program." << endl;
	}
}

void Server::handleEvents() {
//	Check listening sockets
	for (int i = 0; i < countListenSockets; ++i) {
		pollfd& listenSocket = pollFds[i];
		if (listenSocket.revents & POLLIN) {
			acceptClient(listenSocket);
		}
	}
//	Check clients
	for (size_t i = countListenSockets; i < pollFds.size(); ++i) {
		pollfd& clientSocket = pollFds[i];

		if (clientSocket.revents == NoEvents)
			continue;

		Client* client = clientRepository.findBySocketDescriptor(pollFds[i].fd);
		if (clientSocket.revents & POLLIN) {
			clientSocket.events = readClient(client);
//            clientSocket.revents = 0;
		}
		if (clientSocket.revents & POLLOUT) {
			clientSocket.events = writeClient(client);
//            clientSocket.revents = 0;
		}
		if (clientSocket.revents & POLLHUP) {
			disconnectClient(client, true);
			close(clientSocket.fd);
            pollFds.erase(pollFds.begin() + i);
		}
	}
}

void Server::acceptClient(pollfd& listenSocket) {
	struct sockaddr_in address;
	
//	int clientSocketDescriptor = accept(listenSocket.fd, (struct sockaddr*) &address, reinterpret_cast<socklen_t*>(sizeof(sockaddr)));
	socklen_t size = sizeof(sockaddr);
	int clientSocketDescriptor = accept(listenSocket.fd, (struct sockaddr*) &address, &size);
	if (clientSocketDescriptor <= 0) {
		printStatus("cannot accept connection");
		return;
	}

	const string ip = std::string(inet_ntoa(address.sin_addr));
	const string sd = std::to_string(clientSocketDescriptor);
	printStatus("accept connection client " + ip + ", sd = " + sd);

	fcntl(clientSocketDescriptor, F_SETFL, O_NONBLOCK);
	pollFds.push_back(initPollFd(clientSocketDescriptor, POLLIN | POLLOUT | POLLHUP));

	Client* client = new Client(clientSocketDescriptor);
	clientRepository.addClient(client);
}

void Server::disconnectClient(Client* client, bool isRemoveClient) {
	if (client == nullptr)
		return;
	
	struct sockaddr_in address;
	getsockname(client->getSocketDescriptor(), (struct sockaddr*) &address,
				reinterpret_cast<socklen_t*>(sizeof(sockaddr)));

	const string ip = std::string(inet_ntoa(address.sin_addr));
	const string sd = std::to_string(client->getSocketDescriptor());
	printStatus("disconnect client " + ip + ", sd = " + sd);
	
//	cout << "[" << client->getClientIP() << "] has opted to close the connection" << endl;
// TODO должны закрыть pollfd клиента. И удалить из vector pollfds это делается снаружи при обработке POLLHUP
//	close(client->getSocketDescriptor());
	printStatus("client clear()");
//	client->clear();
	if (isRemoveClient) {
		printStatus("remove client");
		clientRepository.removeBySocketDescriptor(client->getSocketDescriptor());
	}
	delete client;
	printStatus("deleted client");
}

short Server::readClient(Client* client) {

	char buffer[BUFFER_SIZE];
//	TODO Хватит ли объёма буфера?
//	std::vector<char> buffer(BUFFER_SIZE);
	ssize_t bytesReadCount = recv(client->getSocketDescriptor(),
								  buffer,
								  BUFFER_SIZE - 1,
								  MsgNoFlag);

	if (bytesReadCount < 0) {
		printStatus("recv() error");
		return POLLHUP;
	}
	if (bytesReadCount == 0) {
		printStatus("recv() timeout");
		return POLLHUP;
	}
	buffer[bytesReadCount] = '\0';
	const string& sd = std::to_string(client->getSocketDescriptor());
	printStatus(">>> " + sd + "\n" + string(buffer));
	cout << "========================" << endl;
//		checkBuffer();
//		TODO передача в parse() vector?
//		Request* request = requestParser.parse(buffer.data());

//		Config* config = configRepository.getConfig(request->getUri(),
//                                                    std::to_string(client->getHostPort()),
//                                                    request->findHeaderByName("Host"));

//		Response response = requestHandler.handle(*request, *config);

//		delete request;
	return POLLOUT;

}

short Server::writeClient(Client* client) {
//	TODO check null?
	printStatus("<<<");
	string buffer = "HTTP/1.1 200 OK\n"
					"Content-Length: 88\n"
					"Connection: close\r\n"
					"\r\n"
					"<html>\n"
					"<body>\n"
					"<h1>Hello, World!</h1>\n"
					"</body>\n"
					"</html>\n";

	ssize_t countSendBytes = send(client->getSocketDescriptor(), buffer.c_str(), buffer.size(),
								  MsgNoFlag);
	return POLLIN;
//	if (!client)
//		return false;

//	requestHandler.formResponse(client);
//	if (!client->getResponse()->toSend.empty()) {

/*
	if (!response->toSend.empty()) {
		string buffer = client->getResponse()->toSend;

		ssize_t countSendBytes = send(client->getSocketDescriptor(), buffer.c_str(), buffer.size(),
									  MsgNoFlag);

		if (countSendBytes < 0) {
			return false;
		}
		if (countSendBytes > 0) {
			client->getResponse()->toSend = buffer.substr(countSendBytes);
			cout << "Sent " << countSendBytes << " bytes to fd: "
					  << client->getSocketDescriptor() << endl;
			if (client->getResponse()->toSend.empty())
				client->update();
		}
	}
 */
	return true;
}

/* TODO uncomment?
 * void Server::closeConnections() {
	nfds_t numberOfClients = clients.size();

	for (nfds_t i = 0; i < numberOfClients; ++i) {
		WebClient*& client = clients[i];

		if (client->getStatus() == WebClient::Close) {
			close(client->getSocketDescriptor());
			delete client;
			clients.erase(clients.begin() + i);
			pollFds.erase(pollFds.begin() + i + countListenSockets);
		}
	}
}*/

void Server::printStatus(const string& message) const {
	cout << Utils::getCurrentTimestamp(true) << ": " << message << endl;
}

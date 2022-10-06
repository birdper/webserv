

#include "Server.hpp"

Server::Server(RequestParser& requestParser,
			   ConfigRepository& settingsRepository) :
		requestParser(requestParser),
		configRepository(settingsRepository) {
}

void Server::initSockets() {
	std::vector<std::pair<string, int> > hosts = configRepository.getHostsForBind();

	for (size_t i = 0; i < hosts.size(); ++i) {
		string& ip = hosts[i].first;
		int port = hosts[i].second;

		Socket& socket = *Socket::getInstance(ip, port);
		socket.init();

		int socketDescriptor = socket.getSocketDescriptor();
		Socket::setNonblockMode(socketDescriptor);

		putListenSocket(socket);
		pollFds.push_back(initPollFd(socketDescriptor, POLLIN));

		Utils::printStatus("Listening socket start on " + ip + ":" + std::to_string(port));
	}
	countListenSockets = listenSockets.size();
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
		acceptConnections();
		handleEvents();
	}
}

#pragma clang diagnostic pop


void Server::polling() {
	int result = poll(pollFds.data(), pollFds.size(), PollInfinityTimeout);
	if (result < 0) {
		perror("Webserv: poll() failure");
		exit(EXIT_FAILURE);
	} else if (result == 0) {
		Utils::printStatus("poll() timed out.");
	}
}

void Server::acceptConnections() {
	for (int i = 0; i < countListenSockets; ++i) {
		pollfd& listenSocket = pollFds[i];
		if (listenSocket.revents & POLLIN) {
			acceptClient(listenSocket);
			listenSocket.revents &= ~POLLIN;
		}
	}
}

void Server::acceptClient(pollfd& listenSocket) {
	struct sockaddr_in address;
	socklen_t size = sizeof(address);

	int clientSocketDescriptor = accept(listenSocket.fd, (struct sockaddr*) &address, &size);
	if (clientSocketDescriptor <= 0) {
		Utils::printStatus("cannot accept connection");
		return;
	}
	Socket::setNonblockMode(clientSocketDescriptor);

	Client* client = new Client(clientSocketDescriptor, listenSocket.fd);
	clientRepository.addClient(client);

	pollFds.push_back(initPollFd(clientSocketDescriptor, POLLIN | POLLOUT | POLLHUP));

	string ip = string(inet_ntoa(address.sin_addr));
	string sd = std::to_string(clientSocketDescriptor);
	Utils::printStatus("accept connection client " + ip + ", sd = " + sd);
}

void Server::handleEvents() {
	for (size_t i = countListenSockets; i < pollFds.size(); ++i) {
		pollfd& clientSocket = pollFds[i];

		if (clientSocket.revents == NoEvents)
			continue;

		Client* client = clientRepository.findBySocketDescriptor(pollFds[i].fd);
		if (clientSocket.revents & POLLHUP) {
			Utils::printStatus("POLLHUP!");
			disconnectClient(client);
			clientSocket.revents &= ~POLLHUP;
		}
		if (clientSocket.revents & POLLIN) {
			clientSocket.revents &= ~POLLIN;
			readClient(client);
			if (client->isReadyRequest()) {
				handleRequest(client);
			}
		}
		if (clientSocket.revents & POLLOUT && client->isReadyRequest()) {
			clientSocket.revents &= ~POLLOUT;
			writeClient(client);
		}
	}
}

void Server::readClient(Client* client) {
	char buffer[BUFFER_SIZE];
	ssize_t bytesReadCount = recv(client->getSocketDescriptor(),
								  buffer,
								  BUFFER_SIZE - 1,
								  MsgNoFlag);

	if (bytesReadCount < 0) {
		Utils::printStatus("PRINT_STATUS() recv() error");
		cerr << "STRERROR: recv() failure: " << strerror(errno) << endl;
		disconnectClient(client);
		return;
	}
	if (bytesReadCount == 0) {
		Utils::printStatus("recv() timeout");
		disconnectClient(client);
		return;
	}
	buffer[bytesReadCount] = '\0';
/*	const string& sd = std::to_string(client->getSocketDescriptor());
	Utils::printStatus(">>> " + sd + "\n" + string(buffer));
	cout << "========================" << endl;
*/
	client->appendToBuffer(string(buffer, bytesReadCount));
	requestParser.parse(client->getRequest(), *client);
	std::cout << std::endl;
}

void Server::handleRequest(Client* client) {
	Config& config = findConfig(*client, client->getRequest());

	Response& response = RequestHandler::getInstance(client->getRequest(), config).handle();

	client->setResponse(&response);
}

Config& Server::findConfig(const Client& client, const Request& request) {
	Socket* listenSocket = listenSockets.find(client.getListenSocketDescriptor())->second;

	VirtualServer virtualServer = configRepository.getServerConfig(listenSocket->getIp(),
																   listenSocket->getPortString(),
																   request.findHeaderValue("Host"));

	Config* config = configRepository.findLocationConfigByUri(virtualServer, request.getUri());
	if (config == nullptr) {
		config = configRepository.getLocationConfig(virtualServer.getParameters());
	}
	return *config;
}

void Server::writeClient(Client* client) {
	Utils::printStatus(" >>>> write client");
	Response* response = client->getResponse();

	string buffer;
	if (client->getBuffer().empty()) {
		buffer = response->serialize();
	} else {
		buffer = client->getBuffer();
	}
	std::cout << "Before: buffer->size(): " << buffer.size() << std::endl;
	ssize_t countSendBytes = send(client->getSocketDescriptor(),
								  buffer.c_str(),
								  buffer.size(),
								  MsgNoFlag);
	std::cout << "send bytes " << countSendBytes << std::endl;

	if (countSendBytes < 0) {
		Utils::printStatus("Client ended the userfd! " + std::to_string(client->getSocketDescriptor()));
	}
	client->setBuffer(buffer.substr(countSendBytes));

	delete response;
	if (client->getRequest().findHeaderValue("Connection") == "close") {
		client->setIsReadyRequest(false);
		disconnectClient(client);
	}
	if (client->getBuffer().empty()) {
		client->setIsReadyRequest(false);
	}
//        printResponse(client->getResponse());
}

void Server::disconnectClient(Client* client) {
	if (client == nullptr)
		return;

	struct sockaddr_in address;
	socklen_t size = sizeof(sockaddr);
	getsockname(client->getSocketDescriptor(), (struct sockaddr*) &address, &size);

	const string ip = std::string(inet_ntoa(address.sin_addr));
	const string sd = std::to_string(client->getSocketDescriptor());

	clientRepository.removeBySocketDescriptor(client->getSocketDescriptor());
	Utils::printStatus("remove client " + ip + ", sd = " + sd);

	for (int i = 0; i < pollFds.size(); ++i) {
		if (pollFds[i].fd == client->getSocketDescriptor()) {
			close(pollFds[i].fd);
			pollFds.erase(pollFds.begin() + i);
			Utils::printStatus("close sd = " + sd);
			break;
		}
	}
}

void Server::putListenSocket(Socket& socket) {
	listenSockets[socket.getSocketDescriptor()] = &socket;
}

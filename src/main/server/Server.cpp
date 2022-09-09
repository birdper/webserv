

#include "Server.hpp"
#include "Request.hpp"

Server::Server(RequestParser& requestParser,
			   RequestHandler& requestHandler,
			   ConfigRepository& settingsRepository) :
		requestParser(requestParser),
		requestHandler(requestHandler),
		settingsRepository(settingsRepository) {}

void Server::initSockets(std::vector<int>& hosts) {
	Socket socket;
	for (size_t i = 0; i < hosts.size(); ++i) {
		int socketDescriptor = socket.init(hosts[i]);
/** Добавляем созданный слушающий сокет в */
		pollFds.push_back(initPollFd(socketDescriptor, POLLIN));
	}
	countListenSockets = pollFds.size();
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

void Server::mainLoop() {
	while (true) {
		polling();
		handleEvents();
	}
}

#pragma clang diagnostic pop

void Server::polling() {
	int result = poll(pollFds.data(), pollFds.size(), InfinityTimeout);
//	TODO read man, what handling errors
	if (result < 0)
		std::cerr << "  poll() failed: " << strerror(errno) << std::endl;
	else if (result == 0)
		std::cerr << "  poll() timed out. End program." << std::endl;
}

void Server::handleEvents() {
//	Check listening sockets
// TODO Если не рабоатет то, возможно, нужно объединить в один цикл
	for (int i = 0; i < countListenSockets; ++i) {
		pollfd& pollfd = pollFds[i];
		if (pollfd.revents != NoEvents && pollfd.events == POLLIN) {
			acceptClient(pollfd);
		}
	}
//	Check clients
	for (size_t i = countListenSockets; i < pollFds.size(); ++i) {
		pollfd& pollfd = pollFds[i];

		if (pollfd.revents == NoEvents)
			continue;

		Client* client = clientRepository.findBySocketDescriptor(pollFds[i].fd);
		if (pollfd.revents & POLLHUP) {
			disconnectClient(client, true);
			pollFds.erase(pollFds.begin() + i);
		}
		else if (pollfd.revents & POLLIN) {
			readClient(client);
		}
		else if (pollfd.revents & POLLOUT) {
			writeClient(client);
		}
	}
}

void Server::acceptClient(pollfd& pollFd) {
	sockaddr_in clientAddress;
	int addressLength = sizeof(clientAddress);
//	for (int i = 0; i < pollFds.size(); ++i) {
//		int clientFd = accept(pollFds[i].fd, nullptr, nullptr);
		int clientFd = accept(pollFd.fd,
							  (sockaddr*)&clientAddress,
							  (socklen_t*)&addressLength);
		if (clientFd <= 0) {
			return;
//			break;
		}
		fcntl(clientFd, F_SETFL, O_NONBLOCK);
		pollFds.push_back(initPollFd(clientFd, POLLIN | POLLOUT | POLLHUP));

		Client* client = new Client(clientFd, &pollFd.fd, clientAddress);
		clientRepository.addClient(client);
//	}
}

void Server::disconnectClient(Client* client, bool isRemoveClient) {
	if (client == nullptr)
		return;
//	std::cout << "[" << client->getClientIP() << "] has opted to close the connection" << std::endl;
	close(client->getSocketDescriptor());
//	delete client->getResponse();
//	delete client->getRequest();
	if (isRemoveClient)
		clientRepository.removeBySocketDescriptor(client->getSocketDescriptor());
	delete client;
}

bool Server::readClient(Client* client) {

//	byte buffer[BUFFER_SIZE];
//	TODO Хватит ли объёма буфера?
	std::vector<char> buffer(BUFFER_SIZE);
	ssize_t bytesReadCount = recv(client->getSocketDescriptor(),
								  buffer.data(),
								  buffer.size(),
								  MsgNoFlag);

	if (bytesReadCount < 0) {
		return false;
	}
	if (bytesReadCount > 0) {
//		checkBuffer();
//		TODO передача в parse() vector?
		Request* request = requestParser.parse(buffer.data());

		Config* config = settingsRepository.getConfig(request->getUri(),
									 std::to_string(client->getHostPort()),
									 request->findHeaderByName("Host"));

		Response response = requestHandler.handle(*request, *config);

//		delete request;
	}
	return true;
}

bool Server::writeClient(Client* client) {
//	TODO check null?
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
			std::cout << "Sent " << countSendBytes << " bytes to fd: "
					  << client->getSocketDescriptor() << std::endl;
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
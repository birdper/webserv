

#include "Server.hpp"
#include "Request.hpp"

Server::Server(RequestParser& requestParser,
			   RequestHandler& requestHandler,
			   ConfigRepository& settingsRepository) :
        requestParser(requestParser),
        requestHandler(requestHandler),
        configRepository(settingsRepository) {}

void Server::initSockets() {
	Socket socket;

    std::vector< std::pair<std::string, int> > hosts = configRepository.getHosts();
	for (size_t i = 0; i < hosts.size(); ++i) {
        int socketDescriptor = socket.init(hosts[i].first, hosts[i].second);
		pollFds.push_back(initPollFd(socketDescriptor, POLLIN));
        std::cout << "Listening socket start on " << hosts[i].first << ":" << hosts[i].second << std::endl;
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
	if (result < 0)
		std::cerr << "  poll() failed: " << strerror(errno) << std::endl;
	else if (result == 0)
		std::cerr << "  poll() timed out. End program." << std::endl;
}

void Server::handleEvents() {
//	Check listening sockets
// TODO Если не рабоатет то, возможно, нужно объединить в один цикл
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
			readClient(client);
            clientSocket.revents = 0;
		}
		if (clientSocket.revents & POLLOUT) {
			writeClient(client);
            clientSocket.revents = 0;
        }
        if (clientSocket.revents & POLLHUP) {
            disconnectClient(client, true);
            pollFds.erase(pollFds.begin() + i);
        }
	}
}

void Server::acceptClient(pollfd& listenSocket) {
		int clientSocketDescriptor = accept(listenSocket.fd, nullptr, nullptr);
		if (clientSocketDescriptor <= 0) {
			return;
		}
		fcntl(clientSocketDescriptor, F_SETFL, O_NONBLOCK);
		pollFds.push_back(initPollFd(clientSocketDescriptor, POLLIN | POLLOUT | POLLHUP));

		Client* client = new Client(clientSocketDescriptor);
		clientRepository.addClient(client);
}

void Server::disconnectClient(Client* client, bool isRemoveClient) {
	if (client == nullptr)
		return;
    std::cout << "Разъединяем клиента" << std::endl;
//	std::cout << "[" << client->getClientIP() << "] has opted to close the connection" << std::endl;
// TODO должны закрыть pollfd клиента. И удалить из vector pollfds это делается снаружи при обработке POLLHUP
	close(client->getSocketDescriptor());
    client->clear();
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
//        ERROR
		return false;
	}
    if (bytesReadCount == 0) {
//        Timeout
        return false;
    }
	if (bytesReadCount > 0) {
        std::cout << "read from client: " << buffer.data() << std::endl;
//		checkBuffer();
//		TODO передача в parse() vector?
//		Request* request = requestParser.parse(buffer.data());

//		Config* config = configRepository.getConfig(request->getUri(),
//                                                    std::to_string(client->getHostPort()),
//                                                    request->findHeaderByName("Host"));

//		Response response = requestHandler.handle(*request, *config);

//		delete request;
	}
	return true;
}

bool Server::writeClient(Client* client) {
//	TODO check null?
    std::cout << "Тут мы пишем" << std::endl;
    std::string buffer = "HTTP/1.1 200 OK\n"
                         "Content-Length: 88\n"
                         "Connection: close\r\n"
                         "\r\n"
                         "<html>\n"
                         "<body>\n"
                         "<h1>Hello, World!</h1>\n"
                         "</body>\n"
                         "</html>";

    ssize_t countSendBytes = send(client->getSocketDescriptor(), buffer.c_str(), buffer.size(),
                                  MsgNoFlag);
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
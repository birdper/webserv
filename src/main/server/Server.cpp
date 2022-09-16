

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
//	TODO read man, what handling errors
    if (result < 0) {
        cerr << "poll() failure: " << strerror(errno) << endl;
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
        if (clientSocket.revents & POLLIN) {
//            clientSocket.events = readClient(client);
            readClient(client);
            clientSocket.revents &= ~POLLIN;
        }
        if (clientSocket.revents & POLLOUT) {
//            clientSocket.events = writeClient(client);
//            if (!client->isReadyRequest()) {
                writeClient(client);
                clientSocket.revents &= ~POLLOUT;
//            }

        }
        if (clientSocket.revents & POLLHUP) {
            disconnectClient(client, true);
            close(clientSocket.fd);
            pollFds.erase(pollFds.begin() + i);
            clientSocket.revents &= ~POLLHUP;
        }
    }
}

short Server::readClient(Client* client) {

//	TODO Хватит ли объёма буфера?
    char buffer[BUFFER_SIZE];
//    std::vector<char> buffer(BUFFER_SIZE);
    ssize_t bytesReadCount = recv(client->getSocketDescriptor(),
                                  buffer,
                                  BUFFER_SIZE - 1,
                                  MsgNoFlag);

    if (bytesReadCount < 0) {
        Utils::printStatus("recv() error");
        cerr << "recv() failure: " << strerror(errno) << endl;
        perror("Webserv: recv() failure");
        return POLLHUP;
    }
    if (bytesReadCount == 0) {
        Utils::printStatus("recv() timeout");
        return POLLHUP;
    }
    buffer[bytesReadCount] = '\0';
//	const string& sd = std::to_string(client->getSocketDescriptor());
//	Utils::printStatus(">>> " + sd + "\n" + string(buffer));
//	cout << "========================" << endl;

//	checkBuffer();
//    client->addToSendQueue(new string(buffer));

    Utils::printStatus("parse request");
    Request* request = requestParser.parse(buffer);
    if (request != nullptr) {

        Config& config = findConfig(*client, *request);

        Response response = RequestHandler::getInstance(*request, config).handle();
        client->setResponse(&response);
        //    Response response = requestHandler.handle(*request, config);
        client->setIsReadyRequest(true);
        Utils::printStatus("handled request");
    } else {
        client->setIsReadyRequest(false);
        Utils::printStatus("request not ready");
    }

    return POLLOUT;
}

Config& Server::findConfig(const Client& client, const Request& request) {

    Socket& listenSocket = *listenSockets.find(client.getListenSocketDescriptor())->second;
    VirtualServer virtualServer = configRepository.getServerConfig(listenSocket.getIp(),
                                                                   listenSocket.getPortString(),
                                                                   request.findHeaderValue("ServerName"));

    Config* config = configRepository.findLocationConfigByUri(virtualServer, request.getUri());
    if (config == nullptr) {
        bool isLocation = false;
        config = new Config(virtualServer.getParameters(), isLocation);
    }
    return *config;
}

short Server::writeClient(Client* client) {
//	TODO check null?
    Utils::printStatus("<<< write");
    string body = "<html>\n"
                  "<body>\n"
                  "<h1>Hello, World!</h1>\n"
                  "</body>\n"
                  "</html>\n";

    string buffer = "HTTP/1.1 200 OK\n"
                    "Content-Length: " + std::to_string(body.size()) + "\n"
                                                                       "Connection: close\r\n"
                                                                       "\r\n" +
                    body;

    ssize_t countSendBytes = send(client->getSocketDescriptor(), buffer.c_str(), buffer.size(),
                                  MsgNoFlag);
    return POLLIN;
    /*
//	if (!client)
//		return false;

//	requestHandler.formResponse(client);
//	if (!client->getResponse()->toSend.empty()) {


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

void Server::disconnectClient(Client* client, bool isShouldRemoveClient) {
    if (client == nullptr)
        return;

    struct sockaddr_in address;
    socklen_t size = sizeof(sockaddr);
    getsockname(client->getSocketDescriptor(), (struct sockaddr*) &address, &size);

    const string ip = std::string(inet_ntoa(address.sin_addr));
    const string sd = std::to_string(client->getSocketDescriptor());
    Utils::printStatus("disconnect client " + ip + ", sd = " + sd);

//	cout << "[" << client->getClientIP() << "] has opted to close the connection" << endl;
// TODO должны закрыть pollfd клиента. И удалить из vector pollfds это делается снаружи при обработке POLLHUP
//	close(client->getSocketDescriptor());
    Utils::printStatus("client clear()");
//	client->clear();
    if (isShouldRemoveClient) {
        Utils::printStatus("remove client");
        clientRepository.removeBySocketDescriptor(client->getSocketDescriptor());
    }
    delete client;
    Utils::printStatus("deleted client");
}

void Server::putListenSocket(Socket& socket) {
    listenSockets[socket.getSocketDescriptor()] = &socket;
}

/* TODO uncomment?
 * void Server::closeConnections() {
	nfds_t numberOfClients = clients.size();

	for (nfds_t i = 0; i < numberOfClients; ++i) {
		WebClient*& client = clients[i];

		if (client->getStatus() == Client::Close) {
			close(client->getSocketDescriptor());
			delete client;
			clients.erase(clients.begin() + i);
			pollFds.erase(pollFds.begin() + i + countListenSockets);
		}
	}
}*/



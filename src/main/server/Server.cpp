

#include "Server.hpp"

Server::Server(RequestParser& requestParser, RequestHandler& requestHandler) :
		requestParser(requestParser),
		requestHandler(requestHandler) {}

void Server::initSockets(std::vector<int>& hosts) {
	Socket socket;
	int socketDescriptor;
	for (size_t i = 0; i < hosts.size(); ++i) {
//		openSockets.push_back(socket->initSocket(hosts[i]));
		socketDescriptor = socket.initSocket(hosts[i]);

		if (socketDescriptor == InvalidSocket) {
			for (int j = 0; j < i; ++j) {
				close(events[j].fd);
			}
			exit(1);
		}

		events.push_back(getPollFd(socketDescriptor, POLLIN));
	}
	numberEvents = hosts.size();
	currentItem = 0;
}

struct pollfd Server::getPollFd(int socketDescriptor, short events) {
	struct pollfd fd;
	fd.fd = socketDescriptor;
	fd.events = events;
	fd.revents = 0;
	return fd;
}

void Server::mainLoop() {

	while (true) {
		polling();
		acceptConnections();
		for (; hasReadyClient(); nextClient()) {
			if (receiveRequest()) {
				requestParser.handleRequest(getClient());
				requestHandler.getResponse(getClient());
			}
			sendResponse(NULL);
		}
		closeConnections();
	}
}

void Server::polling() {
	int result;

//	std::cout << "Waiting on poll()..." << std::endl;
	result = poll(&(events.front()), events.size(), InfinityTimeout);
	if (result < 0)
		std::cerr << "  poll() failed: " << strerror(errno) << std::endl;
	else if (result == 0)
		std::cerr << "  poll() timed out. End program." << std::endl;
}

void Server::acceptConnections() {
//	int clientFd;

	sockaddr_in clientAddress;
	int clientAddrLen = sizeof(clientAddress);
	int clientFd = InvalidSocket;

	for (; currentItem < numberEvents; ++currentItem) {
		while (hasEvents()) {

			clientFd = accept(events[currentItem].fd,
							  (sockaddr*)&clientAddress,
							  (socklen_t*)&clientAddrLen);
			if (clientFd <= 0)
				break;
			fcntl(clientFd, F_SETFL, O_NONBLOCK);

			std::cout << "New incoming connection - fd: " << clientFd << std::endl;

			events.push_back(getPollFd(clientFd, POLLIN | POLLOUT));

//			WebClient* client = new WebClient(clientFd, clientAddress, servers[currentItem]->getHost());
			WebClient* client = new WebClient(clientFd, clientAddress);
			clients.push_back(client);
		}
	}
}

void Server::closeConnections() {
	nfds_t numberOfClients = clients.size();

	for (nfds_t i = 0; i < numberOfClients; ++i) {
		if (clients[i]->getStatus() == WebClient::Close) {
			close(clients[i]->getSocketDescriptor());
			delete clients[i];
			clients.erase(clients.begin() + i);
			events.erase(events.begin() + i + numberEvents);
		}
	}
}

bool Server::receiveRequest() {
	char buffer[1024 * 1024];

	if (isReadable()) {
		WebClient* client = getClient();
		size_t readBytes = recv(client->getSocketDescriptor(),
								buffer,
								sizeof(buffer),
								MsgNoFlag);
		if (readBytes == 0) {
			client->setStatus(WebClient::Close);
		}
		else if (readBytes != std::string::npos) {
			buffer[readBytes] = '\0';
			client->setBuffer(buffer);
			return true;
		}
	}
	return false;
}

bool Server::sendResponse(Response* response) {
	if (isWriteable()) {
		WebClient* client = getClient();
		if (client->getBuffer().empty()) {
//			client->setBuffer(client->getResponse()->toString());
			client->setBuffer(response->toString());
		}
		ssize_t sendBytes = send(client->getSocketDescriptor(),
								 client->getBuffer().c_str(),
								 client->getBuffer().length(),
								 MsgNoFlag);
		if (sendBytes == 0) {
			client->setStatus(WebClient::Close);
		}
		else if (sendBytes != std::string::npos) {
			client->setBuffer(client->getBuffer().substr(sendBytes));
			std::cout << "Sent " << sendBytes << " bytes to fd: " << client->getSocketDescriptor()
					  << std::endl;
			if (client->getBuffer().empty()) {
				client->refresh();
			}
			return true;
		}
	}
	return false;
}

WebClient* Server::getClient() const {
	return clients[currentItem - numberEvents];
}

void Server::nextClient() {
	++currentItem;
}

bool Server::hasReadyClient() {
	if (currentItem < events.size())
		return true;
	currentItem = 0;
	return false;
}

bool Server::hasEvents() const {
	return events[currentItem].revents;
}

bool Server::isReadable() const {
	return hasEvents() & POLLIN && getClient()->getStatus() == WebClient::Readable;
}

bool Server::isWriteable() const {
	return hasEvents() & POLLOUT && getClient()->getStatus() == WebClient::Writing;
}

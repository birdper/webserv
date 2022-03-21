

#include "HttpServer.hpp"

static struct pollfd fillPollfd(int sd, short events) {
	struct pollfd fd;
	fd.fd = sd;
	fd.events = events;
	fd.revents = 0;
	return fd;
}

HttpServer::HttpServer(std::vector<std::string>& hosts) {
	for (size_t i = 0; i < hosts.size(); ++i) {
		SimpleServer* server = new SimpleServer(hosts[i]);
		servers.push_back(server);
		pollfds.push_back(fillPollfd(server->getSocket()->getSd(), POLLIN));
	}
	numberOfListeners = servers.size();
	currentItem = 0;
}

HttpServer::~HttpServer() {}

void HttpServer::polling() {
	int result;

//	std::cout << "Waiting on poll()..." << std::endl;
	result = poll(&(pollfds.front()), pollfds.size(), -1);
	if (result < 0)
		std::cerr << "  poll() failed: " << strerror(errno) << std::endl;
	else if (result == 0)
		std::cerr << "  poll() timed out.  End program." << std::endl;
}

void HttpServer::createConnections() {
	int newFd;

	for (; currentItem < numberOfListeners; ++currentItem) {
		while (isHasEvents()) {
			newFd = accept(pollfds[currentItem].fd, nullptr, nullptr);
			if (newFd <= 0)
				break;
			std::cout << "New incoming connection - fd: " << newFd << std::endl;
			pollfds.push_back(fillPollfd(newFd, POLLIN | POLLOUT));
			clients.push_back(new Client(newFd,
										 servers[currentItem]->getHost()));
		}
	}
}

void HttpServer::closeConnections() {
	nfds_t numberOfClients = clients.size();

	for (nfds_t i = 0; i < numberOfClients; ++i) {
		if (clients[i]->getStatus() == Client::CLOSE) {
			close(clients[i]->getFd());
			delete clients[i];
			clients.erase(clients.begin() + i);
			pollfds.erase(pollfds.begin() + i + numberOfListeners);
		}
	}
}

bool HttpServer::receiveRequest() {
	char buffer[1024 * 1024];

	if (isReadable()) {
		Client* client = getClient();
		size_t readBytes = recv(client->getFd(), buffer, sizeof(buffer), 0);
		if (readBytes == 0) {
			client->setStatus(Client::CLOSE);
		} else if (readBytes != std::string::npos) {
			buffer[readBytes] = '\0';
			client->setBuffer(buffer);
			return true;
		}
	}
	return false;
}

bool HttpServer::toSendResponse() {
	if (isWriteable()) {
		Client* client = getClient();
		if (client->getBuffer().empty())
			client->setBuffer(client->getResponse()->toString());
		ssize_t sendBytes = send(client->getFd(),
								 client->getBuffer().c_str(),
								 client->getBuffer().length(),
								 0);
		if (sendBytes == 0) {
			client->setStatus(Client::CLOSE);
		} else if (sendBytes != std::string::npos) {
			client->setBuffer(client->getBuffer().substr(sendBytes));
			std::cout << "Sent " << sendBytes << " bytes to fd: "
					  << client->getFd() << std::endl;
			if (client->getBuffer().empty())
				client->update();
			return true;
		}
	}
	return false;
}

bool HttpServer::isHasEvents() const {
	return pollfds[currentItem].revents;
}

bool HttpServer::isReadable() const {
	return pollfds[currentItem].revents & POLLIN &&
		   getClient()->getStatus() == Client::READABLE;
}

bool HttpServer::isWriteable() const {
	return pollfds[currentItem].revents & POLLOUT &&
		   getClient()->getStatus() == Client::WRITING;
}

Client* HttpServer::getClient() const {
	return clients[currentItem - numberOfListeners];
}

void HttpServer::nextClient() {
	++currentItem;
}

bool HttpServer::haveRawClients() {
	if (currentItem < pollfds.size())
		return true;
	currentItem = 0;
	return false;
}



#include "Server.hpp"
#include "Request.hpp"

Server::Server(RequestParser& requestParser, RequestHandler& requestHandler) :
		requestParser(requestParser),
		requestHandler(requestHandler) {}

void Server::initSockets(std::vector<int>& hosts) {
	Socket socket;
	for (size_t i = 0; i < hosts.size(); ++i) {
		int socketDescriptor = socket.init(hosts[i]);
		pollFds.push_back(initPollFd(socketDescriptor, POLLIN));
	}
	countListenSockets = pollFds.size();
}

struct pollfd Server::initPollFd(int socketDescriptor, short eventTypes) {
	struct pollfd pollFd;
	pollFd.fd = socketDescriptor;
	pollFd.events = eventTypes;
	pollFd.revents = 0;
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
	int result = poll(&(pollFds.front()), pollFds.size(), InfinityTimeout);
//	TODO read man, what handling errors
	if (result < 0)
		std::cerr << "  poll() failed: " << strerror(errno) << std::endl;
	else if (result == 0)
		std::cerr << "  poll() timed out. End program." << std::endl;
}

void Server::handleEvents() {
	for (int i = 0; i < countListenSockets; ++i) {
		pollfd& pollfd = pollFds[i];
		if (pollfd.revents == NoEvents)
			continue;
		if (pollfd.events == POLLIN) {
			acceptConnections();
			continue;
		}
	}
	for (size_t i = countListenSockets; i < pollFds.size(); ++i) {
		pollfd& pollfd = pollFds[i];
		if (pollfd.revents == NoEvents)
			continue;

		Client* client = clientRepository.findBySocketDescriptor(pollFds[i].fd);
		bool isKeepAliveClient = true;
		if (pollfd.revents & POLLIN) {
			isKeepAliveClient = readClient(client);
		}
		else if (pollfd.revents & POLLOUT) {
			isKeepAliveClient = writeClient(client, nullptr);
		}
//		TODO erase and delete client?
		if (!isKeepAliveClient) {
			disconnectClient(client, true);
			pollFds.erase(pollFds.begin() + i);
			break;
		}
	}
}

void Server::acceptConnections() {
	sockaddr_in clientAddress;
	int addressLength = sizeof(clientAddress);

	for (int i = 0; i < pollFds.size(); ++i) {
/*
		int clientFd = accept(pollFds[i].fd,
							  (sockaddr*)&clientAddress,
							  (socklen_t*)&addressLength);
*/
		int clientFd = accept(pollFds[i].fd,
							  nullptr, // TODO Ринат говорит что 2 и 3 аргументы не нужны
							  nullptr);
		if (clientFd <= 0)
			break;

		fcntl(clientFd, F_SETFL, O_NONBLOCK);

//		pollFds.push_back(initPollFd(clientFd, POLLIN | POLLOUT));
//		TODO Клиент при принятии соединения готов только для чтения, Готов для записи при готовом request
		pollFds.push_back(initPollFd(clientFd,  POLLIN | POLLOUT));

		Client* client = new Client(clientFd, clientAddress);
		clientRepository.addClient(client);
	}
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

	byte buffer[BUFFER_SIZE];
	ssize_t bytesReadCount = recv(client->getSocketDescriptor(), buffer, sizeof(buffer), MsgNoFlag);

	if (bytesReadCount <= 0) {
		return false;
	}
//	Request* request = requestParser.parse(client, buffer, countByteRead);
	Request* request = new Request((byte*)buffer, bytesReadCount);
	handleRequest(client, request);
	delete request;
	return true;
}

void Server::handleRequest(Client* client, Request* request) {

	// Parse the request
	// If there's an error, report it and send a server error in response
	if (!request->parse()) {
		sendStatusResponse(client, Status(BAD_REQUEST), request->getParseError());
		return;
	}

	/*std::cout << "Headers:" << std::endl;
	for(int i = 0; i < req->getNumHeaders(); i++) {
		std::cout << "\t" << req->getHeaderStr(i) << std::endl;
	}
	std::cout << std::endl;*/

	// Send the request to the correct handler function
	switch (request->getMethod()) {
		case GET:
//			handleGet(client, request);
			break;
		case PUT:
//			handleGet(client, request);
			break;
		case POST:
//			handleGet(client, request);
			break;
		case DELETE:
//			handleGet(client, request);
			break;
		default:
//			sendStatusResponse(client, Status(NOT_IMPLEMENTED));
			break;
	}
}

bool Server::writeClient(Client* client, Response* response) {
//	TODO check null?
//	if (!client)
//		return false;

//	requestHandler.formResponse(client);

//	if (!client->getResponse()->toSend.empty()) {
	if (!response->toSend.empty()) {
		string buffer = client->getResponse()->toSend;

		ssize_t countSendBytes = send(client->getSocketDescriptor(), buffer.c_str(), buffer.size(),
									  MsgNoFlag);

		if (countSendBytes <= 0) {
			return false;
		}

		client->getResponse()->toSend = buffer.substr(countSendBytes);
		std::cout << "Sent " << countSendBytes << " bytes to fd: " << client->getSocketDescriptor()
				  << std::endl;
		if (client->getResponse()->toSend.empty())
			client->update();
	}
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


#include "ClientRepository.hpp"

void ClientRepository::addClient(Client* client) {
	clients[client->getSocketDescriptor()] = client;
}

Client* ClientRepository::findBySocketDescriptor(int socketDescriptor) {
	std::map<int, Client*>::iterator foundIt;
	foundIt = clients.find(socketDescriptor);
	if (foundIt == clients.end())
		return nullptr;
	return foundIt->second;
//	TODO отладочный вывод
	std::cerr << "Client not found" << std::cerr;
	return nullptr;
}

void ClientRepository::removeBySocketDescriptor(int socketDescriptor) {
	clients.erase(socketDescriptor);
}


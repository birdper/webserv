

#include "ClientRepository.hpp"

void ClientRepository::addClient(Client* client) {
//	clients.push_back(client);
	clients[client->getSocketDescriptor()] = client;
}

Client* ClientRepository::findBySocketDescriptor(int socketDescriptor) {
	std::map<int, Client*>::iterator foundIt;
	foundIt = clients.find(socketDescriptor);
	if (foundIt == clients.end())
		return nullptr;
	return foundIt->second;
//	for (int pos = 0; pos < clients.size(); ++pos) {
//		if (socketDescriptor == clients[pos]->getSocketDescriptor()) {
//			return clients[pos];
//		}
//	}
//	TODO отладочный вывод
	std::cerr << "Client not found" << std::cerr;
	return nullptr;
}

void ClientRepository::removeBySocketDescriptor(int socketDescriptor) {
	clients.erase(socketDescriptor);
//	std::remove(clients.begin(), clients.end(), socketDescriptor);

//	for (int pos = 0; pos < clients.size(); ++pos) {
//		if (socketDescriptor == clients[pos]->getSocketDescriptor()) {
//			clients.erase(clients.begin() + pos);
//		}
//	}
}


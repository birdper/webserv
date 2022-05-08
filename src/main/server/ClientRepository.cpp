

#include "ClientRepository.hpp"

void ClientRepository::addClient(Client* client) {
	clients.push_back(client);
}

Client* ClientRepository::findBySocketDescriptor(int socketDescriptor) {
	for (int pos = 0; pos < clients.size(); ++pos) {
		if (socketDescriptor == clients[pos]->getSocketDescriptor()) {
			return clients[pos];
		}
	}
//	TODO отладочный вывод
	std::cerr << "Client not found" << std::cerr;
	return nullptr;
}

void ClientRepository::removeBySocketDescriptor(int socketDescriptor) {
	for (int pos = 0; pos < clients.size(); ++pos) {
		if (socketDescriptor == clients[pos]->getSocketDescriptor()) {
			clients.erase(clients.begin() + pos);
		}
	}
}


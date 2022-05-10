#pragma once

#include <iostream>
#include <map>
#include "Client.hpp"

class ClientRepository {
private:
//	std::vector<Client*> clients;
	std::map<int, Client*> clients;

public:
	void addClient(Client* client);
	Client* findBySocketDescriptor(int socketDescriptor);
	void removeBySocketDescriptor(int socketDescriptor);
};
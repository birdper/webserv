#pragma once

#include <iostream>
#include "Client.hpp"

class ClientRepository {
private:
	std::vector<Client*> clients;

public:
	void addClient(Client* client);
	Client* findBySocketDescriptor(int socketDescriptor);
	void removeBySocketDescriptor(int socketDescriptor);
};
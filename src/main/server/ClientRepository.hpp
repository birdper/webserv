#pragma once

#include <iostream>
#include <map>
#include "Client.hpp"

class ClientRepository {
private:
//	std::vector<Client*> clients;
	std::map<int, Client*> clients;
    std::map<string, string> cookie;

public:
	void addClient(Client* client);
	Client* findBySocketDescriptor(int socketDescriptor);
	Client* findByCookie(std::string key);
	void removeBySocketDescriptor(int socketDescriptor);
};
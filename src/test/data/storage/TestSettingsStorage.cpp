
#include <iostream>
#include <vector>

#include "SettingsStorage.hpp"
#include "VirtualServer.hpp"
#include "acutest.h"

void addVirtualServerByHost() {
	SettingsStorage storage;
	VirtualServer server;
	server.setHost("hostVirtual");

	storage.addVirtualServerByHost("namehost", &server);

	TEST_CHECK(storage.findVirtualServer("namehost", "").getHost() == "namehost");
}

void addVirtualServerByHostFewServers() {
	SettingsStorage storage;

	VirtualServer server;
	server.setHost("hostVirtual");

	VirtualServer server1;
	server1.setHost("hostVirtual");

	storage.addVirtualServerByHost(server.getHost(), &server);
	storage.addVirtualServerByHost(server.getHost(), &server1);

	TEST_CHECK(storage.findVirtualServer("hostVirtual", "").getHost() == "hostVirtual");
	TEST_CHECK(storage.getVirtualServers().size() == 1);
	TEST_CHECK(storage.getVirtualServers().begin()->second.size() == 2);
}

void findVirtualServerWhenGivenCorrectPort() {
	SettingsStorage storage;

	VirtualServer server;
	server.setHost("hostVirtual");
	{
		std::vector<std::string> vec;
		vec.push_back("First");
		vec.push_back("Second");
		vec.push_back("Three");
		server.addServerNames(vec);
	}
	VirtualServer server1;
	{
		std::vector<std::string> vec;
		vec.push_back("White");
		vec.push_back("Black");
		server1.addServerNames(vec);
	}	server1.setHost("hostVirtual");

	storage.addVirtualServerByHost(server.getHost(), &server);
	storage.addVirtualServerByHost(server1.getHost(), &server1);
	VirtualServer actual = storage.findVirtualServer("hostVirtual", "Black");

	TEST_CHECK(actual.getServerNames().size() == 2);
	TEST_CHECK(actual.isContainServerName("Black") == true);
	TEST_CHECK(actual.isContainServerName("Second") == false);
}

TEST_LIST = {
		{ "addVirtualServerByHost() Given one VirtualServer",  addVirtualServerByHost },
		{ "addVirtualServerByHost() Given few Virtual Servers",  addVirtualServerByHostFewServers },
		{ "findVirtualServer() Given correct port",  findVirtualServerWhenGivenCorrectPort },

		{nullptr, nullptr}
};


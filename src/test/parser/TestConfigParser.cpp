#include "acutest.h"
#include "ConfigParser.hpp"
#include "ConfigRepositoryImpl.hpp"

static const std::string host = "127.0.0.1:8080";
static const std::string firstServerName = "first_serverName";
static const std::string firstServerRoot = "first_server_root";

static const std::string firstServerLoc1 = "/first/loc1";
static const std::string firstServerLoc1Root = firstServerLoc1 + "/root";

static const std::string firstServerLoc2 = "/first/loc2";
static const std::string firstServerLoc2Root = firstServerLoc2 + "/root";


static const std::string secondServerName = "second_serverName";
static const std::string secondServerRoot = "second_server_root";

static const std::string secondServerLoc1 = "/second/loc1";
static const std::string secondServerLoc1Root = secondServerLoc1 + "/root";

static const std::string secondServerLoc2 = "/second/loc2";
static const std::string secondServerLoc2Root = secondServerLoc2 + "/root";


static const std::string configText =
				"server {\n"
				"	listen " + host + ";\n" +
				"	server_name " + firstServerName + ";\n" +
				"	root " + firstServerRoot + ";\n" +
				"	location " + firstServerLoc1 + " {\n" +
				"		root " + firstServerLoc1Root + ";\n" +
				"	}\n" +
				"	location " + firstServerLoc2 + " {\n" +
				"		root " + firstServerLoc2Root + ";\n" +
				"	}\n" +
				"}\n" +
				"server {\n" +
				"	listen " + host + ";\n" +
				"	server_name " + secondServerName + ";\n" +
				"	root " + secondServerRoot + ";\n" +
				"	location " + secondServerLoc1 + " {\n" +
				"		root " + secondServerLoc1Root + "; \n" +
				"	}\n" +
				"	location " + secondServerLoc2 + " {\n" +
				"		root " + secondServerLoc2Root + "; \n" +
				"	}\n" +
				"}";

void parse() {
	ConfigParser configParser;
	ConfigStorage storage;

	configParser.parseConfig(configText, &storage);
	VirtualServer& foundServer = storage.findVirtualServer(host, secondServerName);

	TEST_CHECK(foundServer.getServerNames()[0] == secondServerName);

//	Parameters* parameters = foundServer.getParameters();
//	TEST_CHECK(parameters->root == "second/www");

	ConfigRepositoryImpl repository(&storage);
	Config& config = *repository.getConfig(secondServerLoc2, host, secondServerName);

	TEST_CHECK(config.getUri() == secondServerLoc2);
	TEST_CHECK(config.getRoot() == secondServerLoc2Root);
}

void wrongServerNameThenReturnDefaultServer() {
	ConfigParser configParser;
	std::string expectedServerName = firstServerName;

	ConfigStorage storage;

	configParser.parseConfig(configText, &storage);
	VirtualServer& server = storage.findVirtualServer(host, "notExistServerName");

	TEST_CHECK(server.getServerNames()[0] == expectedServerName);

	Parameters* parameters = server.getParameters();
	TEST_CHECK(parameters->root == firstServerRoot);

//	ConfigRepositoryImpl repository(&storage);
//	Config& config = *repository.getConfig("/second/site", "127.0.0.1:8080", "second");
//
//	TEST_CHECK(config.getUri() == "/second/site");
//	TEST_CHECK(config.getRoot() == "second/www/html");
}

TEST_LIST = {{"parseConfig()", parse},
			 {"findVirtualServer() when not exist server name then return first server as default", wrongServerNameThenReturnDefaultServer},
			 {nullptr, nullptr}};

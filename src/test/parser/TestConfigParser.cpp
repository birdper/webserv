

#include "acutest.h"
#include "ConfigParser.hpp"
#include "SettingsRepositoryImpl.hpp"

void parse() {
	ConfigParser configParser;

	std::string configText = "server {\n"
							 "	listen 127.0.0.1:8080;\n"
							 "	server_name def;	\n"
							 "	root www;			\n"
							 "	location /site1 {	\n"
							 "		root www/html;	\n"
							 "	}\n"
							 "	location / {		\n"
							 "		root www/site;	\n"
							 "	}\n"
							 "}\n"
							 "server {\n"
							 "	listen 127.0.0.1:8080;	\n"
							 "	server_name second;	\n"
							 "	root second/www;		\n"
							 "	location /second/site {	\n"
							 "		root second/www/html;\n"
							 "	}\n"
							 "	location / { \n"
							 "		root second/www/site;\n"
							 "	}\n"
							 "}";
	SettingsStorage storage;

	configParser.parseConfig(configText, &storage);
	VirtualServer& server = storage.findVirtualServer("127.0.0.1:8080", "second");

	TEST_CHECK(server.getServerNames()[0] == "second");

//	Parameters* parameters = server.getParameters();
//	TEST_CHECK(parameters->root == "second/www");

	SettingsRepositoryImpl repository(&storage);
	Config& config = *repository.getConfig("/second/site", "127.0.0.1:8080", "second");

	TEST_CHECK(config.getUri() == "/second/site");
	TEST_CHECK(config.getRoot() == "second/www/html");
}
void parse1() {
	ConfigParser configParser;

	std::string configText = "server {\n"
							 "	listen 127.0.0.1:8080;\n"
							 "	server_name def;	\n"
							 "	root www;			\n"
							 "	location /site1 {	\n"
							 "		root www/html;	\n"
							 "	}\n"
							 "	location / {		\n"
							 "		root www/site;	\n"
							 "	}\n"
							 "}\n"
							 "server {\n"
							 "	listen 127.0.0.1:8080;	\n"
							 "	server_name second;	\n"
							 "	root second/www;		\n"
							 "	location /second/site {	\n"
							 "		root second/www/html;\n"
							 "	}\n"
							 "	location / { \n"
							 "		root second/www/site;\n"
							 "	}\n"
							 "}";
	SettingsStorage storage;

	configParser.parseConfig(configText, &storage);
	VirtualServer& server = storage.findVirtualServer("127.0.0.1:8080", "notexistname");

	TEST_CHECK(server.getServerNames()[0] == "def");

//	Parameters* parameters = server.getParameters();
//	TEST_CHECK(parameters->root == "second/www");

//	SettingsRepositoryImpl repository(&storage);
//	Config& config = *repository.getConfig("/second/site", "127.0.0.1:8080", "second");
//
//	TEST_CHECK(config.getUri() == "/second/site");
//	TEST_CHECK(config.getRoot() == "second/www/html");
}

TEST_LIST = {
		{"parseConfig()", parse},
		{"findVirtualServer() when not exist server name", parse1},
		{nullptr, nullptr}
};





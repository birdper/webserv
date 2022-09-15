#include <iostream>
#include <sstream>

#include <map>
#include <vector>
#include <arpa/inet.h>
#include "Client.hpp"

/*#include "Tokenizer.hpp"
#include "ConfigParser.hpp"
#include "ConfigStorage.hpp"
#include "ConfigRepositoryImpl.hpp"*/
#include "Client.hpp"
#include "ConfigParser.hpp"
#include "ConfigRepositoryImpl.hpp"
#include "ErrorPagePathNotFoundException.hpp"
#include "Server.hpp"


/*
void testTokenizer(const std::string& input) {
	Tokenizer tokenizer;

	std::istringstream iss(input);
	std::vector<Token> tokens = tokenizer.tokenize(iss);
	for (int i = 0; i < tokens.size(); ++i) {
		std::cout << "type " << tokens[i].typeName;
		std::cout << " content " << tokens[i].content << std::endl;
	}
}

void testParserConfig(const std::string& input) {
	ConfigParser parser;
	ConfigStorage* storage = new ConfigStorage();

	parser.parseConfig(input, storage);
	ConfigRepositoryImpl repository(storage);
	VirtualServer virtualServer = storage->findVirtualServer(
			"8080",
			"");
	Config* config = repository.getConfig("/www/html", "8080", "");
	if (config)
		std::cout << "root: " << config->getRoot() << std::endl;
	else
		std::cout << "Not Found Config" << std::endl;


}

size_t parseUriRequest(const std::string& uriRequest,
					   const std::string& uriLocation) {
	return uriRequest.find(uriLocation, 0);
//	return uriLocation.find(uriRequest, 0);
//	size_t posDot = uri.rfind('.');
//	size_t posSlash = uri.rfind('/');
}

bool isIncorrectConfigFile(const std::string& configFileName) {
	return false;
}
*/

//TODO implement input config file
int main(int argc, char *argv[]) {

	/*
	if (argc != 2) {
		std::cerr << "Did not provide a configuration file" << std::endl;
		exit(1);
	}
	if (isIncorrectConfigFile(argv[1])) {
		std::cerr << "Incorrect config file" << std::endl;
		exit(1);
	}
	*/
    ConfigParser configParser;
    ConfigStorage configStorage;
    configParser.parseConfig(FileReader::readFile("webserv.conf"), &configStorage);

    RequestParser* requestParser;
    ConfigRepositoryImpl configRepository(&configStorage);
    Server server(*requestParser, configRepository);
    server.run();

//    Socket socket1;
//    socket1.init("127.0.0.1", 7000);
//    socket1.init("10.21.21.155", 7000);
//    socket1.init("0.0.0.0", 7000);

//	server.initSockets();
//	server.run();

	std::string input = "server {\n"
						"        listen 8080;\n"
						"\n"
						"\n"
						"        location /www {\n"
						"            error_page 404 /an404.html;\n"
						"            root serv1_loc_www;\n"
						"            client_max_body_size       10m;\n"
						"        }\n"
						"\n"
						"        location /www/htmll {\n"
						"            error_page  404  /404.html;\n"
						"            root  serv1_loc_/www/html;\n"
						"        }\n"
						"\n"
						"        location /www/html/index {\n"
						"            error_page  404  /404.html;\n"
						"            root  serv1_loc_/www/html/index;\n"
						"        }\n"
						"}\n"
						"\n"
						"server {\n"
						"        listen 8080;\n"
						"        server_name hh.ru www.hh.ru;\n"
						"\n"
						"\n"
						"        location /www {\n"
						"            error_page 404 /an404.html;\n"
						"            root serv2_loc_www;\n"
						"            client_max_body_size       10m;\n"
						"        }\n"
						"\n"
						"        location /www/2html {\n"
						"            error_page  404  /404.html;\n"
						"            root  serv2_loc_2html;\n"
						"        }\n"
						"}";
//	testParserConfig(input);
//	testTokenizer();




//	std::cout << parseUriRequest("/first/sec/file.ext",
//								 "/sec")	<< std::endl;
	return 0;
}


#include <vector>

#include "ConfigParser.hpp"
#include "ConfigRepositoryImpl.hpp"
#include "Server.hpp"


bool isIncorrectConfigFile(const std::string& configFileName) {
//  TODO not yet implement
	return false;
}

int main(int argc, char *argv[]) {

//  mplement input config file
	if (argc != 2) {
		std::cerr << "Did not provide a configuration file" << std::endl;
		exit(1);
	}
	if (isIncorrectConfigFile(argv[1])) {
		std::cerr << "Incorrect config file" << std::endl;
		exit(1);
	}

    string configFileName = argv[1];


    ConfigParser* configParser = new ConfigParser();
    ConfigStorage* configStorage = new ConfigStorage();
    configParser->parseConfig(configFileName, configStorage);

    RequestParser* requestParser = new RequestParser();
    ConfigRepositoryImpl* configRepository = new ConfigRepositoryImpl(configStorage);
    Server* server = new Server(*requestParser, *configRepository);
    server->run();
	return 0;
}

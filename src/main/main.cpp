
#include <vector>

#include "ConfigParser.hpp"
#include "ConfigRepositoryImpl.hpp"
#include "Server.hpp"


void validateConfigFile(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Did not provide a configuration file" << std::endl;
        exit(1);
    }
    const std::string configFileName = argv[1];

    if (!Utils::isFileExists(configFileName) || !Utils::isAccessRights(configFileName)) {
        std::cerr << "Incorrect config file" << std::endl;
        exit(1);
    }
}

int main(int argc, char *argv[]) {

    validateConfigFile(argc, argv);

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

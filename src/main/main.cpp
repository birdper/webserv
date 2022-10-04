
#include <vector>

#include "ConfigParser.hpp"
#include "ConfigRepositoryImpl.hpp"
#include "Server.hpp"


void validateConfigFile(const string& configFileName) {
	if (!Utils::isFileExists(configFileName) || !Utils::isAccessRights(configFileName)) {
		std::cerr << "Incorrect config file '" << configFileName << "'." << std::endl;
		exit(1);
	}
}

int main(int argc, char* argv[]) {

	std::string configFileName = "webserv.conf";
	if (argv[1] != nullptr) {
		configFileName = argv[1];
	}

	validateConfigFile(configFileName);

	ConfigParser* configParser = new ConfigParser();
	ConfigStorage* configStorage = new ConfigStorage();
	configParser->parseConfig(configFileName, configStorage);

	RequestParser* requestParser = new RequestParser();
	ConfigRepositoryImpl* configRepository = new ConfigRepositoryImpl(configStorage);
	Server* server = new Server(*requestParser, *configRepository);
	server->run();
	return 0;
}



#include "ParserConfig.hpp"

namespace parser {

ParserConfig::ParserConfig() {
}

ParserConfig::~ParserConfig() {
}

void ParserConfig::printErrMsg(const std::string& msg) {
	std::cerr << "Syntax error: " << msg << std::endl;
	exit(1);
}

void ParserConfig::parseConfig(const std::string& inputString,
							   SettingsStorage* storage) {
// TODO 1: Определить как создаются VirtualServer и Location
	std::istringstream iss(inputString);

	Tokenizer tokenizer;
	std::vector<Token> tokens = tokenizer.tokenize(iss);

	VirtualServer* currentServer;
	Location* location;
	Parameters* currentParams;

//	bool listen;
	bool isHasLocation;
	for (size_t i = 0; i < tokens.size(); ++i) {
		Token& token = tokens[i];
		switch (token.type) {
			case SERVER:
				isHasLocation = false;
				currentServer = new VirtualServer();
				currentParams = currentServer->getParameters();
				break;
			case LOCATION:
				isHasLocation = true;
				location = new Location(
						token.content,
						currentServer->getParameters());
				currentParams = location->getParameters();
				currentServer->addLocation(location);
				break;
			case ENDSERVER:
				if (token.context == NONE_CONTEXT) {
//					if (!listen)
//				printErrMsg("server must have 'listen'");
					if (!isHasLocation)
						printErrMsg("server must have 'location'");
				}
				break;
			case SERVER_NAME:
				currentServer->addServerNames(parseListValues(token.content));
				break;
			case LISTEN:
				storage->addVirtualServerByHost(token.content, currentServer);
				break;
			case AUTOINDEX:
				if (token.content == "on")
					currentParams->hasAutoindexEnabled = true;
				else if (token.content != "off")
					printErrMsg("autoindex may be only 'off' or 'on'");
				break;
			case ROOT:
				currentParams->root = token.content;
				break;
			case REDIRECT:
				currentParams->redirect = token.content;
				break;
			case CLIENT_MAX_BODY_SIZE:
				currentParams->clientMaxBody = token.content;
				break;
			case ERROR_PAGE:
//				TODO parse error pages
//				currentParams->errorPagePaths;
				break;
			case INDEX:
				currentParams->indexFiles = parseListValues(token.content);
				break;
			case FORBIDDEN_METHODS:
				std::vector<std::string> methods = parseListValues(token.content);
				for (int j = 0; j < methods.size(); ++j) {
					if (methods[j] == "POST")
						currentParams->forbiddenMethods.push_back(POST);
					else if (methods[j] == "PUT")
						currentParams->forbiddenMethods.push_back(PUT);
					else if (methods[j] == "DELETE")
						currentParams->forbiddenMethods.push_back(DELETE);
					else
						printErrMsg(
								"Forbidden methods may be only 'PUT', 'POST' or 'DELETE'");
				}
				break;
		}



//		if (token.type == SERVER) {
//			listen = false;
//			currentServer = config.createServer();
//			currentContext = currentServer;
//		} else if (token.type == LOCATION) {
//			location = true;
//			currentContext = currentServer->addLocation(token.content);
//	} else if (token.type == ENDSERVER && token.context == NONE_CONTEXT) {
//			if (!listen)
//				printErrMsg("server must have 'listen'");
//		if (!location)
//			printErrMsg("server must have 'location'");
//	} else {
//		if (token.type == SERVER_NAME) {
//			currentServer->addServerNames(parseListValues(token.content));
//				currentServer->setServerNames(parseListValues(token.content));
//		} else {
//			if (token.type == LISTEN) {
//					listen = true;
//				storage->addVirtualServer(token.content, currentServer);
//					config.addServer(token.content, currentServer);
//			} else if (token.type == AUTOINDEX) {
//				if (token.content != "off" || token.content != "on") {
//					printErrMsg("autoindex may be only 'off' or 'on'");
//				}
//			}
////				currentContext->setParameter(token.typeName, token.content);
//		}
//	}
	}
//	return config;
}

std::vector<std::string>
ParserConfig::parseListValues(const std::string& input) {

	std::istringstream iss(input);
	std::vector<std::string> vector;
	std::string word;
	while (iss >> word) {
		vector.push_back(word);
	}
	return vector;
}

}
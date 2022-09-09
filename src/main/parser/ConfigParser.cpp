

#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {
}

ConfigParser::~ConfigParser() {
}

void ConfigParser::parseConfig(const std::string &inputString,
                               ConfigStorage *storage) {
// TODO 1: Определить как создаются VirtualServer и Location
    std::istringstream iss(inputString);

    Tokenizer tokenizer;
    std::vector<Token> tokens = tokenizer.tokenize(iss);

    VirtualServer *currentServer;
    Location *location;
    Parameters *currentParams;

//	bool listen;
    bool isHasLocation;
    for (size_t i = 0; i < tokens.size(); ++i) {
        Token &token = tokens[i];
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
//					TODO: обязателен ли listen?
//					if (!listen)
//				printErrMsgAndShutDown("server must have 'listen'");
                    if (!isHasLocation)
                        fatalError("server must have 'location'");
                }
                break;
            case SERVER_NAME:
                currentServer->addServerNames(splitStringToVector(token.content));
                break;
            case LISTEN:
                storage->addVirtualServerByHost(token.content, currentServer);
                break;
            case AUTOINDEX:
                if (token.content == "on")
                    currentParams->hasAutoindexEnabled = true;
                else if (token.content != "off")
                    fatalError("autoindex may be only 'off' or 'on'");
                break;
            case ROOT:
                currentParams->root = token.content;
                break;
            case REDIRECT:
                currentParams->redirect = token.content;
                break;
            case CLIENT_MAX_BODY_SIZE:
                currentParams->clientMaxBodySize = token.content;
                break;
            case ERROR_PAGE:
//				TODO parse error pages
//				currentParams->errorPagePaths;
                break;
            case INDEX:
                currentParams->indexFiles = splitStringToVector(token.content);
                break;
            case FORBIDDEN_METHODS:
                std::vector<std::string> methods = splitStringToVector(token.content);
                for (int j = 0; j < methods.size(); ++j) {
                    if (methods[j] == "POST")
                        currentParams->forbiddenMethods.push_back(POST);
                    else if (methods[j] == "PUT")
                        currentParams->forbiddenMethods.push_back(PUT);
                    else if (methods[j] == "DELETE")
                        currentParams->forbiddenMethods.push_back(DELETE);
                    else
                        fatalError("Forbidden methods may be only 'PUT', 'POST' or 'DELETE'");
                }
                break;
        }
    }
}

std::map<std::string, std::string>
ConfigParser::parseErrorPagePaths(const std::string &input) {

    std::map<std::string, std::string> map;
    std::vector<std::string> vec = splitStringToVector(input);


    for (int i = 1; i < vec.size(); i += 2) {
        std::string key = vec[i - 1];
        std::string value = vec[i];
        map[key] = value;
    }
    return map;
}

std::vector<std::string>
ConfigParser::splitStringToVector(const std::string &input) {
    std::vector<std::string> vector;
    std::istringstream iss(input);

    std::string word;
    while (iss >> word) {
        vector.push_back(word);
    }
    return vector;
}

void ConfigParser::fatalError(const std::string &msg) {
    std::cerr << "Syntax error: " << msg << std::endl;
    exit(1);
}

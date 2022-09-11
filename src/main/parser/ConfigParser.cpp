

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
                parseListen(token.content, *currentServer);
                storage->addVirtualServerByHost(currentServer->getIp() + ":" + std::to_string(currentServer->getPort()),
                                                currentServer);
//                storage->addVirtualServerByHost(currentServer->getHost(), currentServer);
                break;
            case AUTOINDEX:
                if (token.content == "on") {
                    currentParams->hasAutoindexEnabled = true;
                } else if (token.content != "off") {
                    fatalError("autoindex may be only 'off' or 'on'");
                }
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

void ConfigParser::parseListen(const std::string &input, VirtualServer &virtualServer) {
    std::vector<std::string> vec = split(input, ":");

    if (vec.size() != 2 && vec.size() != 1) {
        fatalError("Failed to parse listen directive");
    } else if (vec.size() == 1) {
        checkPort(vec[0]);
        virtualServer.setIp("0.0.0.0");
        virtualServer.setPort(std::stoi(vec[0]));
    } else {
        std::string ip = vec[0];
        std::string port = vec[1];
        checkIpAndPort(ip, port);
        virtualServer.setIp(ip);
        virtualServer.setPort(std::stoi(port));
    }
//    TODO здесь сетится строкиа без 0.0.0.0
    virtualServer.setHost(rtrim(input, " "));
}

bool ConfigParser::checkPort(const std::string &str) {
    if (!isDigits(rtrim(str, " "))) {
        fatalError("Failed to parse 'listen'. Port must be number from 1 to 65355");
    }
    try {
        int port = std::stoi(str);
        if (port <= 0 || port > 65535) {
            fatalError("Failed to parse 'listen'. Port must be number from 1 to 65355");
        }
    } catch (std::out_of_range &ex) {
        fatalError("Failed to parse 'listen'. Port must not be greater than 65535");
    }
}

bool ConfigParser::checkIpAndPort(const std::string &ipStr, const std::string &portStr) {
    std::vector<std::string> ip = split(ipStr, ".");
    if (ip.size() != 4) {
        fatalError("The listen directive must have the format ip:port or only port");
    }
    for (int i = 0; i < ip.size(); ++i) {
        if (!isDigits(ip[i])) {
            fatalError(
                    "Failed to parse ip from 'listen'. The IPv4 address must have the format X.X.X.X Where X is a number.");
        }
        try {
            int octet = std::stoi(ip[i]);
            if (octet < 0 || octet > 255) {
                fatalError("Failed to parse ip from 'listen'. Ip octet must be number from 0 to 255");
            }
        }
        catch (std::out_of_range &ex) {
            fatalError("IP must not be greater than 255");
        }
    }
    checkPort(portStr);
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

std::vector<std::string> ConfigParser::split(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t previousPosition = 0;
    size_t currentPosition = 0;
    do {
        currentPosition = str.find(delimiter, previousPosition);
        if (currentPosition == std::string::npos) {
            currentPosition = str.length();
        }
        std::string token = str.substr(previousPosition, currentPosition - previousPosition);
        if (!token.empty()) {
            tokens.push_back(token);
        }
        previousPosition = currentPosition + delimiter.length();
    } while (currentPosition < str.length() && previousPosition < str.length());

    return tokens;
}

std::string ConfigParser::rtrim(std::string str, const std::string &chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

bool ConfigParser::isDigits(const std::string &str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

void ConfigParser::fatalError(const std::string &msg) {
    std::cerr << "Syntax error: " << msg << std::endl;
    exit(1);
}

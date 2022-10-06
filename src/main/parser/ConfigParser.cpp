

#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {
}

ConfigParser::~ConfigParser() {
}

void ConfigParser::parseConfig(const std::string &configFileName,
                               ConfigStorage *storage) {

    string configString = FileReader::readFile(configFileName);
    std::istringstream iss(configString);

    Tokenizer tokenizer;
    std::vector<Token> tokens = tokenizer.tokenize(iss);

    VirtualServer* currentServer;
    Location* location;
    Parameters* currentParams;

    bool isHasLocation;
    for (size_t i = 0; i < tokens.size(); ++i) {
        Token &token = tokens[i];
        switch (token.type) {
            case SERVER:
                isHasLocation = false;
                currentServer = new VirtualServer();
                currentParams = &currentServer->getParameters();
                break;
            case LOCATION:
                isHasLocation = true;
                location = new Location(
                        token.content,
                        currentServer->getParameters());
                currentParams = &location->getParameters();
                currentServer->addLocation(location);
                break;
            case BLOCK_END:
                if (token.context == NONE_CONTEXT) {
//					TODO: обязателен ли listen?
//					if (!listen)
//				printErrMsgAndShutDown("server must have 'listen'");
                    if (!isHasLocation)
                        fatalError("server must have 'location'");
                }
                break;
            case SERVER_NAME:
                currentServer->addServerNames(Utils::split(token.content, " "));
                break;
            case LISTEN:
                parseListen(token.content, *currentServer);
                storage->addVirtualServerByHost(std::to_string(currentServer->getPort()),
                                                currentServer);
                break;
            case AUTOINDEX:
                if (token.content == "on") {
                    currentParams->hasAutoindexEnabled = true;
                } else if (token.content != "off") {
                    fatalError("autoindex may be only 'off' or 'on'");
                }
                break;
            case INDEX:
                currentParams->indexNameFiles = Utils::split(token.content, " ");
                break;
            case ROOT:
                currentParams->root = token.content;
                break;
            case PATH_CGI:
                currentParams->pathCGI = token.content;
                break;
            case EXTENSION_CGI:
                currentParams->extensionCGI = token.content;
                break;
            case REDIRECT:
				parseRedirect(currentParams, token.content);
                break;
            case CLIENT_MAX_BODY_SIZE:
                currentParams->clientMaxBodySize = token.content;
                break;
			case UPLOAD_DIRECTORY:
				currentParams->uploadDirectory = token.content;
				break;
            case ERROR_PAGE:
                parseErrorPagePaths(currentParams, token.content);
                break;
            case ALLOWED_METHODS:
                std::vector<std::string> methods = Utils::split(token.content, " ");
                for (int j = 0; j < methods.size(); ++j) {
                    if (methods[j] == "post")
                        currentParams->allowedMethods.push_back(POST);
                    else if (methods[j] == "put")
                        currentParams->allowedMethods.push_back(PUT);
                    else if (methods[j] == "delete")
                        currentParams->allowedMethods.push_back(DELETE);
                    else
                        fatalError("Forbidden methods may be only 'put', 'post' or 'delete'");
                }
                break;
        }
    }
}

void ConfigParser::parseListen(const std::string &input, VirtualServer &virtualServer) {
    std::vector<std::string> vec = Utils::split(input, ":");

    if (vec.size() != 2 && vec.size() != 1) {
        fatalError("Failed to parse listen directive");
    }

	std::string ip;
	std::string port;
	if (vec.size() == 1) {
        checkPort(vec[0]);
        ip = "0.0.0.0";
        port = vec[0];
    } else {
        ip = vec[0];
        port = vec[1];
        checkIpAndPort(ip, port);
    }

    virtualServer.setIp(ip);
    virtualServer.setPort(std::stoi(port));
    virtualServer.setHost(ip + ":" + port);
}

void ConfigParser::checkPort(const std::string &str) {
	std::string errorMessagePortMustBeNumber = "Failed to parse 'listen'. Port must be number from 1 to 65355";

	if (!isDigits(Utils::rtrim(str, " "))) {
        fatalError(errorMessagePortMustBeNumber);
    }
    try {
        int port = std::stoi(str);
        if (port <= 0 || port > 65535) {
            fatalError(errorMessagePortMustBeNumber);
        }
    } catch (std::out_of_range &ex) {
        fatalError("Failed to parse 'listen'. Port must not be greater than 65535");
    }
}

void ConfigParser::checkIpAndPort(const std::string &ipStr, const std::string &portStr) {
    std::vector<std::string> ip = Utils::split(ipStr, ".");
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

void ConfigParser::parseRedirect(Parameters* params, const std::string& input) {
	std::vector<std::string> redirect = Utils::split(input, " ");
	if (redirect.size() == 2) {
		try {
			params->redirectCode = Utils::stringToInt(redirect[0], 10);
			if (params->redirectCode != 307 && params->redirectCode != 308
					&& (params->redirectCode <= 300 || params->redirectCode >= 305)) {
				fatalError("The redirect code can only be: 301, 302, 304, 307, 308");
			}
		} catch (NumberFormatException &ex) {
			cerr << ex.what() << endl;
			fatalError("The redirect code can only be: 301, 302, 304, 307, 308");
		}
		params->redirectUri = redirect[1];
	} else {
		fatalError("Directive redirect must be: \'return <code> <uri>;\'");
	}
}

void ConfigParser::parseErrorPagePaths(Parameters* params, const std::string& input) {
    std::vector<std::string> errorPage = Utils::split(input, " ");
    if (errorPage.size() == 2) {
        params->addErrorPage(errorPage[0], errorPage[1]);
    } else {
		fatalError("Directive error_page must be: \'error_page <code> <error_page_file_name>;\'");
	}
}

bool ConfigParser::isDigits(const std::string &str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

void ConfigParser::fatalError(const std::string &msg) {
    std::cerr << "Syntax error: " << msg << std::endl;
    exit(1);
}


#include "Tokenizer.hpp"

Tokenizer::Tokenizer() :
		context(NONE_CONTEXT) {
	mapperParameters["unknown_type"] = UNKNOWN_TYPE;
	mapperParameters["server"] = SERVER;
	mapperParameters["listen"] = LISTEN;
	mapperParameters["server_name"] = SERVER_NAME;
	mapperParameters["}"] = BLOCK_END;
	mapperParameters["location"] = LOCATION;
	mapperParameters["root"] = ROOT;
	mapperParameters["upload_directory"] = UPLOAD_DIRECTORY;
	mapperParameters["index"] = INDEX;
	mapperParameters["autoindex"] = AUTOINDEX;
	mapperParameters["error_page"] = ERROR_PAGE;
	mapperParameters["cgi_extension"] = EXTENSION_CGI;
	mapperParameters["cgi_path"] = PATH_CGI;
	mapperParameters["return"] = REDIRECT;
	mapperParameters["client_max_body_size"] = CLIENT_MAX_BODY_SIZE;
	mapperParameters["allowed_methods"] = ALLOWED_METHODS;
}

std::vector<Token> Tokenizer::tokenize(std::istringstream& configText) {

	std::vector<Token> tokens;
	std::string line;
	std::string content;

	while (std::getline(configText, line, delimiter)) {
		std::istringstream issLine(line);
		std::string word;
		while (issLine >> word) {
			TokenType type;
			type = findTokenType(word);
			switch (type) {
				case SERVER:
					defineServer(issLine);
					break;
				case LOCATION:
					content = defineLocation(issLine);
					break;
				case BLOCK_END:
					 defineCloseBrace();
					break;
				case UNKNOWN_TYPE:
					std::cout << "Syntax error: Undefined word: '" << word << "'"
							  << std::endl;
					exit(1);
				default:
					content = getDirectiveContent(issLine);
					break;
			}
			Token currentToken(
					content,
					type,
					static_cast<Context>(context)
					);
			tokens.push_back(currentToken);
			content.clear();
			if (type == SERVER || type == LOCATION) {
				++context;
			}
			word.clear();
		}
	}
	if (context) {
		printErrMsgAndShutDown("Expected close curve brace");
	}
	return tokens;
}

TokenType Tokenizer::findTokenType(const std::string& key) {
	std::map<std::string, TokenType>::iterator it = mapperParameters.find(key);
	if (it == mapperParameters.end()) {
		return UNKNOWN_TYPE;
	}
	return it->second;
}

std::string
Tokenizer::getDirectiveContent(std::istringstream& issLine) {

	std::string word;
	std::string content;
	if (!context)
	{
		printErrMsgAndShutDown("directives must be in the context of the server");
	}
	if (issLine >> word) {
		content.append(word);
	}
	while (issLine >> word) {
		issLine >> word;
		content.append(" ");
		content.append(word);
	}
	return content;
}

void Tokenizer::defineServer(std::istringstream& issLine) {
	if (context != NONE_CONTEXT) {
		printErrMsgAndShutDown("The server block cannot be nested");
	}
	std::string token;
	issLine >> token;

	if (token != "{") {
		printErrMsgAndShutDown("expected the opening curve brace");
	}
	token.clear();
}

std::string Tokenizer::defineLocation(std::istringstream& issLine) {
	if (context != SERVER_CONTEXT) {
		printErrMsgAndShutDown("location block must be in the context of the server");
	}
	std::string uri;
	issLine >> uri;
	if (uri == "{") {
		printErrMsgAndShutDown("expected uri location");
	}
	std::string token;
	issLine >> token;

	if (token != "{") {
		printErrMsgAndShutDown("expected the opening curve brace");
	}
	return uri;
}

void Tokenizer::defineCloseBrace() {
	if (context == NONE_CONTEXT) {
		std::cout << "Syntax error: '}'" << std::endl;
		exit(1);
	}
	--context;
}

void Tokenizer::printErrMsgAndShutDown(const std::string& msg) {
	std::cerr << "Syntax error: " << msg << std::endl;
	exit(1);
}

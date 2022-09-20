#pragma once

#include <iostream>

enum TokenType {
	UNKNOWN_TYPE,
	SERVER,
	LOCATION,
	ENDSERVER,
	LISTEN,
	SERVER_NAME,
	AUTOINDEX,
	INDEX,
	ROOT,
	UPLOAD_DIRECTORY,
	ACCESS_LOG,
	ERROR_LOG,
	REDIRECT,
	PATH_CGI,
	EXTENSION_CGI,
	CLIENT_MAX_BODY_SIZE,
	ERROR_PAGE,
	FORBIDDEN_METHODS
};

enum Context {
	NONE_CONTEXT, SERVER_CONTEXT, LOCATION_CONTEXT
};

struct Token {
	std::string typeName;
	std::string content;
	TokenType type;
	Context context;

	Token();
	Token(const std::string& content,
          TokenType type,
		  Context context);

	Token(const std::string& id,
          const std::string& content,
          TokenType type,
		  Context context);
private:
	Token(const std::string& id);
	Token(const std::string& id, const std::string& content);
	Token(const std::string& id, const std::string& content, TokenType type);
	Token(TokenType type, const std::string& content);
};
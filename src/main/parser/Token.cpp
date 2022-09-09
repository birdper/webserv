#include "Token.hpp"

Token::Token() {}

Token::Token(TokenType type, const std::string& content) {
	this->type = type;
	this->content = content;
}

Token::Token(const std::string& id, const std::string& content) {
	this->typeName = id;
	this->content = content;
}

Token::Token(const std::string& id,
			 const std::string& content,
			 TokenType type) {
	this->typeName = id;
	this->type = type;
	this->content = content;
}

Token::Token(const std::string& id,
			 const std::string& content,
			 TokenType type,
			 Context context) {
	this->typeName = id;
	this->type = type;
	this->content = content;
	this->context = context;
}

Token::Token(const std::string& id) {
	this->typeName = id;
}

Token::Token(const std::string& content, TokenType type, Context context) :
		content(content),
		type(type),
		context(context) {
}

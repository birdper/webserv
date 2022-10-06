#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include "Token.hpp"

class Tokenizer {

private:
	static const char delimiter = ';';
	int context;
	std::map<std::string, TokenType> mapperParameters;

public:
	Tokenizer();
	std::vector<Token> tokenize(std::istringstream& configText);


private:
	TokenType findTokenType(const std::string& key);
	std::string getDirectiveContent(std::istringstream& issLine);
	void defineCloseBrace();
	void defineServer(std::istringstream& issLine);
	std::string defineLocation(std::istringstream& issLine);
	void printErrMsgAndShutDown(const std::string& msg);
};

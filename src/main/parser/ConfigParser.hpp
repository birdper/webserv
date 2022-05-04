

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

#include "Tokenizer.hpp"

#include "Parameters.hpp"

#include "Location.hpp"
#include "VirtualServer.hpp"
#include "SettingsStorage.hpp"

class ConfigParser {

public:
	explicit ConfigParser();
	~ConfigParser();

	void parseConfig(const std::string& inputString, SettingsStorage* storage);

private:
	std::vector<std::string> parseListValues(const std::string& input);
	void printErrMsg(const std::string&);

};
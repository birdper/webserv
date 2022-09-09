

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

#include "Tokenizer.hpp"

#include "Parameters.hpp"

#include "Location.hpp"
#include "VirtualServer.hpp"
#include "ConfigStorage.hpp"

class ConfigParser {

public:
	explicit ConfigParser();
	~ConfigParser();

	void parseConfig(const std::string& inputString, ConfigStorage* storage);

private:
	std::vector<std::string> splitStringToVector(const std::string& input);
	void fatalError(const std::string &msg);

    std::map<std::string, std::string> parseErrorPagePaths(const std::string &input);
};
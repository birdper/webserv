

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
    std::map<std::string, std::string> parseErrorPagePaths(const std::string &input);

    void parseListen(const std::string &input, VirtualServer &virtualServer);
    bool checkPort(const std::string &str);
    bool checkIpAndPort(const std::string &ipStr, const std::string &portStr);

	std::vector<std::string> splitStringToVector(const std::string& input);
    std::vector<std::string> split(const std::string &str, const std::string &delimiter);
    std::string rtrim(std::string str, const std::string &chars);
    bool isDigits(const std::string &str);

	void fatalError(const std::string &msg);
};

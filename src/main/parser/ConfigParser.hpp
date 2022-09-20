

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

#include "Tokenizer.hpp"
#include "Parameters.hpp"
#include "Location.hpp"
#include "VirtualServer.hpp"
#include "ConfigStorage.hpp"
#include "Utils.hpp"
#include "FileReader.hpp"

class ConfigParser {

public:
	explicit ConfigParser();
	~ConfigParser();

	void parseConfig(const std::string& configFileName, ConfigStorage* storage);

private:
    std::map<std::string, std::string> parseErrorPagePaths(const std::string &input);

    void parseListen(const std::string &input, VirtualServer &virtualServer);
    bool checkPort(const std::string &str);
    bool checkIpAndPort(const std::string &ipStr, const std::string &portStr);

    std::string rtrim(std::string str, const std::string &chars);
    bool isDigits(const std::string &str);

	void fatalError(const std::string &msg);
};

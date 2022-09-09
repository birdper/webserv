#pragma once

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include "Client.hpp"
#include "Config.hpp"
#include "../util/Utils.hpp"
#include "../util/FileReader.hpp"
#include "../exception/FileNotFoundException.hpp"

class RequestHandler {
private:
//	Config &config;

public:
//	RequestHandler(Config& config);
	Response handle(Request& request, Config& config);
private:
	bool validate(Request& request, Config& config);
	void handleGet(Request& request, Response& response, Config& config);
	void handlePost(Request& request);

	std::string getPathFromUri(const std::string& uri, Config& config) const;
	std::string getAutoindexBody(std::vector<std::string> fileNames,
								 const std::string& path,
								 const std::string& uri);
	std::vector<std::string> getFileNamesFromDirectory(const std::string& path);
	void readfile(Response& response, const std::string& path);
};
//
// Created by nbirdper on 28.01.2022.
//

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "HttpMethod.hpp"

class Parameters {
public:
	typedef std::map<std::string, std::string> MapErrorPagePaths;

	bool hasAutoindexEnabled;
	bool hasUploadEnabled;

//	TODO Возможно не нужны т.к. относятся к VirtualServer и нужны для Networking
	std::string ip;
	std::string port;
	std::string serverName;

	std::string uri;
	std::string root;
	std::string redirect;
	std::string pathCGI;
	std::string extensionCGI;
	std::string clientMaxBody;

	MapErrorPagePaths errorPagePaths;
	std::vector<std::string> indexFiles;
	std::vector<HttpMethod> forbiddenMethods;
};
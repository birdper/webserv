//
// Created by Aleksei S on 04.10.2022.
//

#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <unistd.h>
#include <fcntl.h>

#include "Request.hpp"
#include "FileReader.hpp"

#define CGI_OUTPUT "outputCGI"

class CGIHandler {
private:
	static const int CHILD_PID = 0;
	static const int FAILURE_CODE = -1;
	string _cgiName;

public:
	std::string handle(const string& cgiName, const string& pathToFile, Request* request);
	std::string getBody();

private:
	void executeCgi(const string& cgiName, const string& pathToFile, Request* request);
	std::string cgiParent(pid_t pid);
	void addToEnv(char** env, const string& newEnvValue);
	string generateNewEnv(const string& key, const string& value);
	char** generateEnv(Request* request);
	long calculateFileSize(const string& fileName) const;
};

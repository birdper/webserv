//
// Created by Aleksei S on 07.09.2022.
//

#pragma once

#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "usings.hpp"
#include <vector>

class Utils {
public:
	static string getCurrentTimestamp(bool localtime = false, const string& format = "%Y-%m-%d %T");
    static void printStatus(const string& message);
    static bool isDirectory(const string& pathToFile);
	static bool isAccessRights(const string& pathToFile);
	static bool isFileExists(const string& pathToFile);
	static time_t getFileModificationDate(const string& pathToFile);
    static std::vector<string> split(const string& str, const string& delimiter);
};

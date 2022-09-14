//
// Created by Aleksei S on 07.09.2022.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "usings.hpp"

class Utils {
public:
	static std::string getCurrentTimestamp(bool localtime = false, const std::string& format = "%Y-%m-%d %T");
    static void printStatus(const string& message);
    static bool isDirectory(const std::string& pathToFile);
	static bool isAccessRights(const std::string& pathToFile);
	static bool isFileExists(const std::string& pathToFile);
	static time_t getFileModificationDate(const std::string& pathToFile);
};

#endif

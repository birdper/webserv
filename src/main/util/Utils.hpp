//
// Created by Aleksei S on 07.09.2022.
//

#pragma once

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>
#include "usings.hpp"

class Utils {
public:
	static string getCurrentTimestamp(bool localtime = false, const string& format = "%Y-%m-%d %T");
    static void printStatus(const string& message);

    static bool isDirectory(const string& pathToFile);
	static bool isAccessRights(const string& pathToFile);
	static bool isFileExists(const string& pathToFile);
	static time_t getFileModificationDate(const string& pathToFile);
    /** for parsers */
    static std::vector<string> split(const string& str, const string& delimiter);
    static string ft_toLower(const string& line);
    static string ltrim(string str, const string& chars);
    static string rtrim(string str, const string& chars);
    static string getExtension(const string& fileName);
    static string getFileName(const string& path);
};

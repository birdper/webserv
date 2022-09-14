#pragma once

#include <iostream>
#include <fstream>
#include "../exception/FileNotFoundException.hpp"


class FileReader {
public:
	static std::string readFile(const std::string& fileName);

    static
    std::string file_contents(const std::string &path);

    std::string readFile2(const std::string &fileName);
};

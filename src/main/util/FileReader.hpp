#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <iostream>
#include <fstream>

class FileReader {
public:
	static std::string readFile(const std::string& fileName);
};

#endif

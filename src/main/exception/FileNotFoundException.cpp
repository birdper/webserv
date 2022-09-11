#include "FileNotFoundException.hpp"

FileNotFoundException::FileNotFoundException(const std::string &fileName) :
		fileName(fileName) {}

const char* FileNotFoundException::what() const throw() {
	return (new std::string("File " + fileName + " not found"))->c_str();
}

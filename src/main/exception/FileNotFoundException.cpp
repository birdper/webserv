#include "FileNotFoundException.hpp"

FileNotFoundException::FileNotFoundException(std::string& fileName) :
		fileName(fileName) {}

const char* FileNotFoundException::what() const throw() {
	return (new std::string("File " + fileName + " not found"))->c_str();
}

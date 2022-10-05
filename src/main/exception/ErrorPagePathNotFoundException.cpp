#include "ErrorPagePathNotFoundException.hpp"

ErrorPagePathNotFoundException::ErrorPagePathNotFoundException(const std::string& path) :
		path(path) {}

const char* ErrorPagePathNotFoundException::what() const throw() {
	return (new std::string(path + " not found"))->c_str();
}

ErrorPagePathNotFoundException::~ErrorPagePathNotFoundException() throw() {

}

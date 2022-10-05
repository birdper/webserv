//
// Created by Nicolle Birdperson on 10/5/22.
//

#include "NumberFormatException.hpp"

NumberFormatException::NumberFormatException(const std::string& cause) :
		_cause(cause) {
}

const char* NumberFormatException::what() const throw() {
	return _cause.c_str();
}

NumberFormatException::~NumberFormatException() throw() {

}

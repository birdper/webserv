//
// Created by Nicolle Birdperson on 9/15/22.
//

#include "ParseRequestException.hpp"

ParseRequestException::ParseRequestException(const std::string& cause) :
        _cause(cause) {
}

const char* ParseRequestException::what() const throw() {
    return _cause.c_str();
}

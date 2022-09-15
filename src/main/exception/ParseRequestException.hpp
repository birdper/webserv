//
// Created by Nicolle Birdperson on 9/15/22.
//

#pragma once

#include <iostream>
#include <exception>

class ParseRequestException : public  std::exception {
private:
    const std::string& _cause;
public:
    ParseRequestException(const std::string& cause);
    const char* what() const throw();
};



#pragma once

#include <iostream>
#include <exception>

class FileNotFoundException : public std::exception {
private:
    const std::string& fileName;
public:
    explicit FileNotFoundException(const std::string& fileName);
    const char* what() const throw();

};

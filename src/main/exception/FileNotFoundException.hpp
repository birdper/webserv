#pragma once

#include <iostream>
#include <exception>

class FileNotFoundException : public std::exception {
private:
    const std::string _cause;

public:
    explicit FileNotFoundException(const std::string& cause);
    const char* what() const throw();
	~FileNotFoundException() _NOEXCEPT ;
};

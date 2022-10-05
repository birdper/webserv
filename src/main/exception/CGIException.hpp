#pragma once

#include <iostream>
#include <exception>

class CGIException : public std::exception {
private:
    const std::string _cause;

public:
    explicit CGIException(const std::string& cause);
    const char* what() const throw();
	~CGIException() _NOEXCEPT;
};

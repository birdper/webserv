#pragma once

#include <exception>
#include <iostream>

class ErrorPagePathNotFoundException : public std::exception {
private:
	const std::string& path;

public:
	explicit ErrorPagePathNotFoundException(const std::string& path);
	const char* what() const throw();

};

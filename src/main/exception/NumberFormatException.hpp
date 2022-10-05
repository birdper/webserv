//
// Created by Nicolle Birdperson on 10/5/22.
//

#pragma once

#include <iostream>
#include <vector>
#include <exception>

class NumberFormatException : public std::exception {
private:
	std::string _cause;
public:
	explicit NumberFormatException(const std::string& cause);
	virtual ~NumberFormatException() _NOEXCEPT;
	const char* what() const throw();
};

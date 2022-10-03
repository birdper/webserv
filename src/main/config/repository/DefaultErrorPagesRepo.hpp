#pragma once

#include <iostream>
#include <map>

class DefaultErrorPagesRepo {
private:
	std::map<std::string, std::string> errors;

public:
	DefaultErrorPagesRepo();
	std::string& getDescriptionErrorByCode(const std::string& code);
};

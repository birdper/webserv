#pragma once

#include <iostream>
#include <map>

class DefaultStatusCodeRepo {
private:
	std::map<std::string, std::string> errors;

public:
	DefaultStatusCodeRepo();
	std::string& getDescriptionByCode(const std::string& code);
};

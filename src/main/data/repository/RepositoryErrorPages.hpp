#pragma once

#include <iostream>
// TODO выбрать map или vector
#include <map>
#include <vector>
//#include "EnumErrorCode.hpp"

class RepositoryErrorPages {

private:
	std::map<std::string, std::string> errors;
//	std::vector<std::string> errorsEnum;

public:
	RepositoryErrorPages();

//	std::string& getContentErrorByEnumCode(EnumErrorCode code);
	std::string& getContentErrorByCode(const std::string& code);
};
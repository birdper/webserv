//
// Created by Nicolle Birdperson on 9/23/22.
//

#pragma once
#include <map>
#include <iostream>

using std::string;

class MimeTypesRepo {
private:
    std::map<string, string> _extensionTypeMap;
    std::map<string, string> _typeExtensionMap;
public:
    MimeTypesRepo();
    string getTypeByExtension(const string& extension);
    string getExtensionByHttpType(const string& httpType);
};


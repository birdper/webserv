//
// Created by Nicolle Birdperson on 9/14/22.
//

#pragma once

#include <unistd.h>
#include <dirent.h>

#include "BaseHandler.hpp"

class BaseHandler;

class GetHandler : public BaseHandler {

private:
    Request* _request;
    Config* _config;

public:
    GetHandler();
    static BaseHandler* getInstance(Request& request, Config& config);
    virtual ~GetHandler();

    Response handle(Request& request, Config& config);

private:
    explicit GetHandler(Request& request, Config& config);
    string getAutoindexBody(std::vector<string> fileNames, const string& path, const string& uri);
    std::vector<string> getFileNamesFromDirectory(const string& path);
    std::string getPathFromUri() const;
};

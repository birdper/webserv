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

    Response handle();

private:
    explicit GetHandler(Request& request, Config& config);
    string getAutoindexBody(const string& path, const string& uri);
    std::vector<string> getFileNamesFromDirectory(const string& path);
    std::string handleDirectory() const;
    string findPathToIndexFile(string& root) const;
    void handleDirectory(Response& response, string& path);
    void handleFile(Response& response, string& path);
    string getErrorPage(const string& errorCode);
};

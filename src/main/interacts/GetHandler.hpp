//
// Created by Nicolle Birdperson on 9/14/22.
//

#pragma once

#include <unistd.h>
#include <dirent.h>

#include "BaseHandler.hpp"
#include "MimeTypesRepo.hpp"

class BaseHandler;

class GetHandler : public BaseHandler {
private:
    Request* _request;
    Config* _config;
    MimeTypesRepo& _mimeTypesRepo;

public:
    static BaseHandler* getInstance(Request& request, Config& config, MimeTypesRepo& mimeTypeRepo);
    virtual ~GetHandler();

    void handle(Response& response);

private:
    explicit GetHandler(Request& request, Config& config, MimeTypesRepo& mimeTypesRepo);
    string getAutoindexBody(const string& path, const string& uri);
    std::vector<string> getFileNamesFromDirectory(const string& path);
    string findPathToIndexFile(string& root) const;
    void handleDirectory(Response& response, string& path);
    void handleFile(Response& response, string& path);
    string getErrorPage(const string& errorCode);
    void setBodyToResponse(Response& response, const string& extension, const string& body);
};

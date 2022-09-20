#pragma once

#include "usings.hpp"
#include "GetHandler.hpp"
#include "PostHandler.hpp"

class RequestHandler {
private:
    std::map<string, BaseHandler*> _methods;
    Request& _request;
    Config& _config;

public:
    virtual ~RequestHandler();
    static RequestHandler getInstance(Request& request, Config& config);
	Response handle();

private:
    RequestHandler(Request& request, Config& config);
    bool validate(Response& response);


/*    std::string getPathFromUri(const std::string& uri, Config& config) const;
    std::string getAutoindexBody(std::vector<std::string> fileNames,
								 const std::string& path,
								 const std::string& uri);
    std::vector<std::string> getFileNamesFromDirectory(const std::string& path);
    void readfile(Response& response, const std::string& path);*/
};
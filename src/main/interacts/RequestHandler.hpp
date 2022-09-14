#pragma once

#include "usings.hpp"
#include "GetHandler.hpp"
#include "PostHandler.hpp"

class RequestHandler {
private:
    std::map<string, BaseHandler*> _methods;

public:
    RequestHandler();
    virtual ~RequestHandler();
	Response handle(Request& request, Config& config);

private:
    bool validate(Request& request, Config& config);


/*    std::string getPathFromUri(const std::string& uri, Config& config) const;
    std::string getAutoindexBody(std::vector<std::string> fileNames,
								 const std::string& path,
								 const std::string& uri);
    std::vector<std::string> getFileNamesFromDirectory(const std::string& path);
    void readfile(Response& response, const std::string& path);*/
};
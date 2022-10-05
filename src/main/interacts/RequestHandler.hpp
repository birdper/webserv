#pragma once

#include "usings.hpp"
#include "GetHandler.hpp"
#include "PostHandler.hpp"
#include "DeleteHandler.hpp"
#include "MimeTypesRepo.hpp"
#include "../cgi/CGIHandler.hpp"

class RequestHandler {
private:
    std::map<string, BaseHandler*> _methods;
    Request& _request;
    Config& _config;

public:
    virtual ~RequestHandler();
    static RequestHandler getInstance(Request& request, Config& config);
	Response& handle();

private:
    RequestHandler(Request& request, Config& config);
    bool isValidResponse(Response& response);
    string getErrorPageBody(const string& errorCode);
    void setErrorPageBody(Response& response);
    void setStatusCodeAndDescription(Response& response);
	void handleCGI(Response& response) const;
	bool isCgiRequest();
	bool isErrorResponse(Response& response);
};

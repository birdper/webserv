#pragma once

#include "usings.hpp"
#include "GetHandler.hpp"
#include "PostHandler.hpp"
#include "DeleteHandler.hpp"
#include "MimeTypesRepo.hpp"

class RequestHandler {
private:
    std::map<string, BaseHandler*> _methods;
    Request& _request;
    Config& _config;

public:
    virtual ~RequestHandler();
    static RequestHandler getInstance(Request& request, Config& config);
	void handle(Response& response);

private:
    RequestHandler(Request& request, Config& config);
    bool isValidResponse(Response& response);
    string getErrorPage(const string& errorCode);
    void setErrorPageBodyIfHas(Response& response);
    void setStatusCodeAndDescription(Response& response);
};

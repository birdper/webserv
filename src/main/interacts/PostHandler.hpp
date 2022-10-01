//
// Created by Nicolle Birdperson on 9/14/22.
//


#pragma once

#include "BaseHandler.hpp"


class PostHandler : public BaseHandler {
private:
    Request& _request;

public:
    static BaseHandler* getInstance(Request& request, Config& config);
    virtual ~PostHandler();

    void handle(Response& response);

private:
    PostHandler(Request& request, Config& config);

	bool saveBodyToFile(const string& pathToFile, const string& content);
};


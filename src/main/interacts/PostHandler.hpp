//
// Created by Nicolle Birdperson on 9/14/22.
//


#pragma once

#include "BaseHandler.hpp"


class PostHandler : public BaseHandler {
private:
    Request& _request;
    Config& _config;

public:
    static BaseHandler* getInstance(Request& request, Config& config);
    virtual ~PostHandler();

    Response handle(Request& request, Config& config);

private:
    PostHandler(Request& request, Config& config);

};


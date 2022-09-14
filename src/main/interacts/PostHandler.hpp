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
    PostHandler(Request& request, Config& config);
    virtual ~PostHandler();

    static BaseHandler* getInstance();
    Response handle(Request& request, Config& config);


};


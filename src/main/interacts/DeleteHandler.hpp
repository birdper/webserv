//
// Created by Nicolle Birdperson on 10/3/22.
//

#pragma once

#include "BaseHandler.hpp"

class DeleteHandler : BaseHandler {
public:
    static BaseHandler* getInstance(Request& request, Config& config);
    ~DeleteHandler();
    void handle(Response& response);

private:
    DeleteHandler(Request& request, Config& config);
    bool removeFile(const string& fileName);
};


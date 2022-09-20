//
// Created by Nicolle Birdperson on 9/14/22.
//

#pragma once

#include <unistd.h>
#include "usings.hpp"
#include "Utils.hpp"
#include "FileReader.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"


class BaseHandler {
public:
    BaseHandler();
    virtual ~BaseHandler();

    virtual Response handle() = 0;

//    virtual  BaseHandler* getInstance(Request& request, Config& config) = 0;
protected:

    void readfileToBody(Response& response, const std::string& path);
    std::string getRedirectPageBody(std::pair<int, std::string> redirect);

    string getResourcePath(const string& locationUri,
                           string root,
                           const string& requestUri) const;
};

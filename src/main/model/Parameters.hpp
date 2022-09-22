//
// Created by nbirdper on 28.01.2022.
//

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "HttpMethod.hpp"

class Parameters {
public:
    typedef std::map<std::string, std::string> MapErrorPagePaths;

    bool hasAutoindexEnabled;
    bool hasUploadEnabled;

    std::string uri;
    std::string root;
    std::string redirect;
    std::string pathCGI;
    std::string extensionCGI;
    std::string clientMaxBodySize;

    MapErrorPagePaths errorPagePaths;
    std::vector<std::string> indexNameFiles;
    std::vector<HttpMethod> forbiddenMethods;

    Parameters() {}

    Parameters(const Parameters& p)
            :
            hasAutoindexEnabled(p.hasAutoindexEnabled),
            hasUploadEnabled(p.hasUploadEnabled),
            uri(p.uri),
            root(p.root),
            redirect(p.redirect),
            pathCGI(p.pathCGI),
            extensionCGI(p.extensionCGI),
            clientMaxBodySize(p.clientMaxBodySize)
//            errorPagePaths(p.errorPagePaths),
//            indexNameFiles(p.indexNameFiles),
//            forbiddenMethods(p.forbiddenMethods)
            {
    }

    Parameters& operator=(const Parameters& p) {
        if (this != &p) {
            hasAutoindexEnabled = p.hasAutoindexEnabled;
            hasUploadEnabled = p.hasUploadEnabled;
            uri = p.uri;
            root = p.root;
            redirect = p.redirect;
            pathCGI = p.pathCGI;
            extensionCGI = p.extensionCGI;
            clientMaxBodySize = p.clientMaxBodySize;
//            errorPagePaths = p.errorPagePaths;
//            indexNameFiles = p.indexNameFiles;
//            forbiddenMethods = p.forbiddenMethods;
        }
        return *this;
    }

};
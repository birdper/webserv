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
    std::string uploadDirectory;

    MapErrorPagePaths errorPagePaths;
    std::vector<std::string> indexNameFiles;
    std::vector<HttpMethod> allowedMethods;

    Parameters() {
	    allowedMethods.push_back(GET);
	}

    Parameters(const Parameters& p)
            :
		    hasAutoindexEnabled(p.hasAutoindexEnabled),
		    hasUploadEnabled(p.hasUploadEnabled),
		    uri(p.uri),
		    root(p.root),
		    redirect(p.redirect),
		    pathCGI(p.pathCGI),
		    extensionCGI(p.extensionCGI),
		    clientMaxBodySize(p.clientMaxBodySize),
		    uploadStorePath(p.uploadStorePath),
		    errorPagePaths(p.errorPagePaths),
		    indexNameFiles(p.indexNameFiles),
		    allowedMethods(p.allowedMethods)
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
	        uploadDirectory = p.uploadDirectory;
            errorPagePaths = p.errorPagePaths;
            indexNameFiles = p.indexNameFiles;
	        allowedMethods = p.allowedMethods;
        }
        return *this;
    }

    void addErrorPage(const std::string& key, const std::string& value) {
        errorPagePaths.insert(std::make_pair(key, value));
    }

};
//
// Created by nbirdper on 28.01.2022.
//

#pragma once

#include <map>
#include <vector>
#include "usings.hpp"
#include "Parameters.hpp"
#include "../exception/ErrorPagePathNotFoundException.hpp"
#include "MimeTypesRepo.hpp"
#include "../config/repository/DefaultStatusCodeRepo.hpp"

class Config {
private:
	Parameters* _parameters;
	MimeTypesRepo* _mimeTypesRepo;
	DefaultStatusCodeRepo* _statusCodeRepo;
    bool _isLocationConfig;

public:

//	explicit Config(Parameters& parameters);
//    explicit Config(Parameters& parameters, bool isLocation);
	Config(Parameters& parameters,
		   MimeTypesRepo* mimeTypesRepo,
		   DefaultStatusCodeRepo* defaultStatusCodeRepo,
		   bool isLocationConfig);

	bool isLocationConfig() const;
	bool isMethodAllowed(HttpMethod method) const;
	bool isAutoindexEnabled() const;
	bool isUploadEnabled() const;
    bool isCGI();

    const string& getLocationUri() const;
    const string& getUploadDirectory() const;
	const string& getRoot() const;
	int getRedirectCode() const;
	const string& getRedirectUri() const;
	const string& getPathCGI() const;
	const string& getExtensionCGI() const;
	const string& getClientMaxBodySize() const;
	const string& getErrorPageRelativePath(const string& errorCode) const;
	std::vector<string> getIndexFiles() const;
	string getPathErrorPageWithRoot(const string& errorCode);

	string getMimeTypeByExtension(const string& extension) const;
    string findCustomErrorPage(const string& errorCode);
    string getDefaultErrorPage(const string& errorCode);
    string getDescriptionByCode(const string& code);
};
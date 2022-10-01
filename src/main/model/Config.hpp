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
#include "../config/repository/DefaultErrorPagesRepo.hpp"

class Config {
private:
	Parameters* _parameters;
	MimeTypesRepo* _mimeTypesRepo;
	DefaultErrorPagesRepo* _defaultErrorPagesRepo;
    bool _isLocationConfig;

public:

//	explicit Config(Parameters& parameters);
//    explicit Config(Parameters& parameters, bool isLocation);
	Config(Parameters& parameters,
	       MimeTypesRepo* mimeTypesRepo,
	       DefaultErrorPagesRepo* defaultErrorPagesRepo,
	       bool isLocationConfig);

	bool isLocationConfig() const;
	bool isMethodAllowed(HttpMethod method) const;
	bool isAutoindexEnabled() const;
	bool isUploadEnabled() const;
    bool isCGI();

    const string& getLocationUri() const;
    const string& getUploadStorePath() const;
	const string& getRoot() const;
	const string& getRedirect() const;
	const string& getPathCGI() const;
	const string& getExtensionCGI() const;
	const string& getClientMaxBody() const;
	const string& getErrorPageRelativePath(const string& errorCode) const;
	std::vector<string> getIndexFiles() const;
	string getPathErrorPageWithRoot(const string& errorCode);

	string getMimeTypeByExtension(const string& extension) const;
};
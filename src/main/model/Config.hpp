//
// Created by nbirdper on 28.01.2022.
//

#pragma once

#include <map>
#include <vector>
#include "usings.hpp"
#include "Parameters.hpp"
#include "../exception/ErrorPagePathNotFoundException.hpp"

class Config {
private:
	Parameters* _parameters;
    bool _isLocationConfig;

public:
	explicit Config(Parameters& parameters);
    explicit Config(Parameters& parameters, bool isLocation);

    bool isLocationConfig() const;
    bool isMethodAllowed(HttpMethod method) const;
	bool isAutoindexEnabled() const;
	bool isUploadEnabled() const;
    bool isCGI();

    const string& getLocationUri() const;
	const string& getRoot() const;
	const string& getRedirect() const;
	const string& getPathCGI() const;
	const string& getExtensionCGI() const;
	const string& getClientMaxBody() const;
	const string& getErrorPageRelativePath(const string& errorCode) const;
	std::vector<string> getIndexFiles() const;
	string getPathErrorPageWithRoot(const string& errorCode);
};
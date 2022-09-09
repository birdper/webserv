//
// Created by nbirdper on 28.01.2022.
//

#pragma once

#include <map>
#include <vector>
#include "Parameters.hpp"
#include "../exception/ErrorPagePathNotFoundException.hpp"

class Config {

private:
	Parameters* parameters;

public:
	Config(Parameters* parameters);

	bool isMethodAllowed(HttpMethod method) const;
	bool isAutoindexEnabled() const;
	bool isUploadEnabled() const;

	const std::string& getIp() const;
	const std::string& getPort() const;
	const std::string& getServerName() const;
	const std::string& getUri() const;
	const std::string& getRoot() const;
	const std::string& getRedirect() const;
	const std::string& getPathCGI() const;
	const std::string& getExtensionCGI() const;
	const std::string& getClientMaxBody() const;
	const std::string& getErrorPageRelativePath(const std::string& errorCode) const;
	std::vector<std::string>& getIndexFiles() const;
	std::string getPathErrorPageWithRoot(const std::string& errorCode);
};
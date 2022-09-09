//
// Created by nbirdper on 28.01.2022.
//

#include "Config.hpp"

Config::Config(Parameters* parameters) :
		parameters(parameters) {}

bool Config::isMethodAllowed(HttpMethod method) const {
	std::vector<HttpMethod>::iterator it = std::find(parameters->forbiddenMethods.begin(),
													 parameters->forbiddenMethods.end(),
													 method);
	if (*it == method)
		return false;
	return true;
}

bool Config::isAutoindexEnabled() const {
	return parameters->hasAutoindexEnabled;
}

bool Config::isUploadEnabled() const {
	return parameters->hasUploadEnabled;
}

const std::string& Config::getIp() const {
	return parameters->ip;
}

const std::string& Config::getPort() const {
	return parameters->port;
}

const std::string& Config::getServerName() const {
	return parameters->serverName;
}

const std::string& Config::getUri() const {
	return parameters->uri;
}

const std::string& Config::getRoot() const {
	return parameters->root;
}

const std::string& Config::getRedirect() const {
	return parameters->redirect;
}

const std::string& Config::getPathCGI() const {
	return parameters->pathCGI;
}

const std::string& Config::getExtensionCGI() const {
	return parameters->extensionCGI;
}

const std::string& Config::getClientMaxBody() const {
	return parameters->clientMaxBodySize;
}

const std::string& Config::getErrorPageRelativePath(const std::string& errorCode) const {
	Parameters::MapErrorPagePaths::iterator it =
			parameters->errorPagePaths.find(errorCode);
	if (it == parameters->errorPagePaths.end()) {
		throw ErrorPagePathNotFoundException(errorCode);
//		return nullptr;
	}
	return it->second;
}

std::string Config::getPathErrorPageWithRoot(const std::string& errorCode) {
		std::string path = getErrorPageRelativePath(errorCode);
		if (path.at(0) == '/') {
			return getRoot() + path;
		}
		else {
			return getRoot() + getUri() + path;
		}
}

std::vector<std::string>& Config::getIndexFiles() const {
	return parameters->indexFiles;
}
//
// Created by nbirdper on 28.01.2022.
//

#include "Config.hpp"

/*
Config::Config(Parameters& parameters) :
		_parameters(&parameters) {}

Config::Config(Parameters& parameters, bool isLocation) :
		_parameters(&parameters),
		_isLocationConfig(isLocation) {
}
*/

Config::Config(Parameters& parameters,
               MimeTypesRepo* mimeTypesRepo,
               DefaultErrorPagesRepo* defaultErrorPagesRepo,
               bool isLocationConfig) :
		_parameters(&parameters),
		_mimeTypesRepo(mimeTypesRepo),
		_defaultErrorPagesRepo(defaultErrorPagesRepo),
		_isLocationConfig(isLocationConfig) {}

bool Config::isMethodAllowed(HttpMethod method) const {
	std::vector<HttpMethod>::const_iterator it = std::find(_parameters->forbiddenMethods.begin(),
	                                                       _parameters->forbiddenMethods.end(),
	                                                       method);
	if (it == _parameters->forbiddenMethods.end())
		return true;
	return false;
}

bool Config::isAutoindexEnabled() const {
	return _parameters->hasAutoindexEnabled;
}

bool Config::isUploadEnabled() const {
	return _parameters->hasUploadEnabled;
}

const string& Config::getLocationUri() const {
	return _parameters->uri;
}

const string& Config::getRoot() const {
	return _parameters->root;
}

const string& Config::getRedirect() const {
	return _parameters->redirect;
}

const string& Config::getPathCGI() const {
	return _parameters->pathCGI;
}

const string& Config::getExtensionCGI() const {
	return _parameters->extensionCGI;
}

const string& Config::getClientMaxBody() const {
	return _parameters->clientMaxBodySize;
}

const string& Config::getErrorPageRelativePath(const string& errorCode) const {
	Parameters::MapErrorPagePaths::const_iterator it =
			_parameters->errorPagePaths.find(errorCode);
	if (it == _parameters->errorPagePaths.end()) {
		throw ErrorPagePathNotFoundException(errorCode);
//		return nullptr;
	}
	return it->second;
}

string Config::getPathErrorPageWithRoot(const string& errorCode) {
	string path = getErrorPageRelativePath(errorCode);
	if (path.at(0) == '/') {
		return getRoot() + path;
	} else {
		return getRoot() + getLocationUri() + path;
	}
}

std::vector<string> Config::getIndexFiles() const {
	return _parameters->indexNameFiles;
}

bool Config::isLocationConfig() const {
	return _isLocationConfig;
}

bool Config::isCGI() {
	return !getPathCGI().empty();
}

const string& Config::getUploadStorePath() const {
	return _parameters->uploadStorePath;
}

string Config::getMimeTypeByExtension(const string& extension) const {
	return _mimeTypesRepo->getMimeTypeByExtension(extension);
}

//
// Created by nbirdper on 28.01.2022.
//

#include "Location.hpp"

Location::Location(const std::string& uri,
				   Parameters* parentParams) {
    _uri = uri;
    _params = *parentParams;
//	params.uri = uri;
//	params.root = parentParams->root;
//	params.redirect = parentParams->redirect;
//	params.pathCGI = parentParams->pathCGI;
//	params.extensionCGI = parentParams->extensionCGI;
//	params.errorPagePaths = parentParams->errorPagePaths;
//	params.indexNameFiles = parentParams->indexNameFiles;
//	params.forbiddenMethods = parentParams->forbiddenMethods;
}

Parameters* Location::getParameters() {
	return &_params;
}

std::string Location::getUri() const {
	return _uri;
}

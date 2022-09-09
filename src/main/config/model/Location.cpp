//
// Created by nbirdper on 28.01.2022.
//

#include "Location.hpp"

Location::Location(const std::string& uri,
				   Parameters* parentParams) {
	params.uri = uri;
	params.root = parentParams->root;
	params.redirect = parentParams->redirect;
	params.pathCGI = parentParams->pathCGI;
	params.extensionCGI = parentParams->extensionCGI;
	params.errorPagePaths = parentParams->errorPagePaths;
	params.indexFiles = parentParams->indexFiles;
	params.forbiddenMethods = parentParams->forbiddenMethods;
}

Parameters* Location::getParameters() {
	return &params;
}

const std::string& Location::getUri() const {
	return params.uri;
}

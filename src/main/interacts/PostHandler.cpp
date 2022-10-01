//
// Created by Nicolle Birdperson on 9/14/22.
//

#include "PostHandler.hpp"

BaseHandler* PostHandler::getInstance(Request& request, Config& config) {
	return new PostHandler(request, config);
}

PostHandler::PostHandler(Request& request, Config& config) : BaseHandler(config),
		_request(request) {
}

PostHandler::~PostHandler() {

}

void PostHandler::handle(Response& response) {
	if (!_config.isMethodAllowed(_request.getHttpMethod())) {
		response.setStatusCode("405 Method Not Allowed");
		return;
	}

	if (!_config.isUploadEnabled()) {
		response.setStatusCode("405 Method Not Allowed");
		return;
	}

	string fileName = getResourcePath(_config.getLocationUri(), _config.getUploadStorePath(), _request.getUri());
	if (!saveBodyToFile(fileName, _request.getBody())) {
		response.setStatusCode("500 Can't save file");
		return;
	}
	setBodyToResponse(response, fileName, _request.getBody());
}

bool PostHandler::saveBodyToFile(const string& pathToFile, const string& content) {
	std::ofstream targetFile(pathToFile);
	if (!targetFile.is_open()) {
		return false;
	}

	targetFile << content;
	targetFile.close();
	return true;
}

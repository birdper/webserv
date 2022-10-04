//
// Created by Nicolle Birdperson on 9/14/22.
//

#include "PostHandler.hpp"

BaseHandler* PostHandler::getInstance(Request& request, Config& config) {
	return new PostHandler(request, config);
}

PostHandler::PostHandler(Request& request, Config& config) :
        BaseHandler(request, config) {
}

PostHandler::~PostHandler() {

}

void PostHandler::handle(Response& response) {
	Utils::printStatus("POST HANDLER!");

	if (_config.getUploadStorePath().empty()) {
		Utils::printStatus("Upload directory not defined");
		response.setStatusCode("405");
		return;
	}

	string fileName = getResourcePath(_config.getLocationUri(), _config.getUploadStorePath(), _request.getUri());
	Utils::printStatus("POST HANDLER: getResourcePath: " + fileName);

	if (!saveBodyToFile(fileName, _request.getBody())) {
		Utils::printStatus("POST HANDLER: 500 Can't save file: " + fileName);
		response.setStatusCode("500");
		return;
	}
	Utils::printStatus("POST HANDLER: 201 Created: " + fileName);

	response.setStatusCode("201");
	string extension = Utils::getExtension(fileName);
	string contentType = _config.getMimeTypeByExtension(extension);

	response.addHeader("Content-Type", contentType);
	if (!_request.findHeaderValue("Transfer-Encoding").empty()) {
		response.addHeader("Transfer-Encoding", "chunked");
		response.setBody(_request.getBuffer());
	} else {
		response.addHeader("Content-Length", std::to_string(_request.getBody().size()));
		response.setBody(_request.getBody());
	}
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

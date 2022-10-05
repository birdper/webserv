//
// Created by Nicolle Birdperson on 9/14/22.
//

#include "BaseHandler.hpp"

BaseHandler::BaseHandler(Request& request, Config& config) :
        _request(request),
        _config(config) {}

BaseHandler::~BaseHandler() {

}

string BaseHandler::getResourcePath(const string& locationUri,
                                    const string& root,
                                    const string& requestUri) const {
	unsigned long endPos = locationUri.rfind("/");
	if (endPos != 0) {
		return root + requestUri.substr(endPos, requestUri.length());
	} else {
		return root + requestUri;
	}
}

void BaseHandler::setBodyToResponse(Response& response,
                                   const string& extension,
                                   const string& body) {

	string contentType = _config.getMimeTypeByExtension(extension);

	response.setBody(body);
	response.addHeader("Content-Type", contentType);
	response.addHeader("Content-Length", std::to_string(body.size()));
}

void BaseHandler::readfileToBody(Response& response, const std::string& path) {
    std::string extension = path.substr(path.find_last_of("/\\") + 1);
    std::string fileExtension = path.substr(path.find_last_of('.') + 1);

//	response.setContentType(mimeType(extension));
    try {
        response.setBody(FileReader::readFile(path));
        response.setStatusCode("200");
    } catch (FileNotFoundException& ex) {
        Utils::printStatus(ex.what());
//		response.setBody(getErrorPageBody("ДОБРО ПОЖАЛОВАТЬ НА СТРАНИЦУ 404!"));
        response.setStatusCode("404");
    }
}

/*bool validateFile(const string& path) {
    if (Utils::isFileExists(path) && !Utils::isDirectory(path) &&
        !access(path.c_str(), W_OK)) {
        return true;
    }
    return false;
}*/

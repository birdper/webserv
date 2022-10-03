//
// Created by Nicolle Birdperson on 9/14/22.
//

#include "BaseHandler.hpp"

BaseHandler::BaseHandler(Config& config) :
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

void BaseHandler::setErrorResponse(Response& response, const string& errorCode) {
    string description = _config.getDescriptionErrorByCode(errorCode);
    string errorPageFileName = _config.findCustomErrorPage(errorCode);
    response.setStatusCode(errorCode + " " + description);
    string body = getErrorPage(errorCode);
    if (!body.empty()) {
        string extension = Utils::getExtension(errorPageFileName);
        setBodyToResponse(response, extension, body);
    }
}

string BaseHandler::getErrorPage(const string& errorCode) {
    string errorPageFileName = _config.findCustomErrorPage(errorCode);
    string body;
    if (!errorPageFileName.empty()) {

        string root = _config.getRoot();
        if (errorPageFileName.front() != '/' && root.back() != '/') {
            root.append("/");
        }

        string pathToErrorPage = root + errorPageFileName;
        body = FileReader::readFile(pathToErrorPage);
    } else {
        body = _config.getDefaultErrorPage(errorCode);
    }

    return body;
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

//TODO АДАПТИРОВАТЬ
std::string BaseHandler::getRedirectPageBody(std::pair<int, std::string> redirect) {
    int redirectCode = redirect.first;

    if ((redirectCode > 300 && redirectCode < 303) || redirectCode == 307 || redirectCode == 308) {
        std::string redirectMsg = std::to_string(redirectCode);
        return "<html>\n"
               "<head><title>" + redirectMsg + "</title></head>\n" + "<body>\n" + "<center><h1>" +
               redirectMsg + "</h1></center>\n" + "<hr><center>webserv</center>\n" + "</body>\n" +
               "</html>";
    } else
        return redirect.second;
}

bool validateFile(const string& path) {
    if (Utils::isFileExists(path) && !Utils::isDirectory(path) &&
        !access(path.c_str(), W_OK)) {
        return true;
    }
    return false;
}

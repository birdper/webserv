//
// Created by Nicolle Birdperson on 9/14/22.
//

#include "BaseHandler.hpp"

BaseHandler::BaseHandler(Config& config) : _config(config) {}

BaseHandler::~BaseHandler() {

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
                                   const string& path,
                                   const string& body) {

//	std::cout << "path: " <<  path << std::endl;

	string extension = Utils::getExtension(path);
//	std::cout << "extension: " <<  extension<< std::endl;
	string contentType = _config.getMimeTypeByExtension(extension);
//	std::cout << "contentType: " <<  contentType << std::endl;


	response.setBody(body);
	response.addHeader("Content-Type", contentType);
	response.addHeader("Content-Length", std::to_string(body.size()));
}

bool validateFile(const string& path) {
    if (Utils::isFileExists(path) && !Utils::isDirectory(path) &&
        !access(path.c_str(), W_OK)) {
        return true;
    }
    return false;
}

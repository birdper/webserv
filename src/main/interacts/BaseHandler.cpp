//
// Created by Nicolle Birdperson on 9/14/22.
//

#include "BaseHandler.hpp"

BaseHandler::BaseHandler() {
}

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
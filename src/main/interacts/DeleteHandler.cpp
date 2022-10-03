//
// Created by Nicolle Birdperson on 10/3/22.
//

#include "DeleteHandler.hpp"

BaseHandler* DeleteHandler::getInstance(Request& request, Config& config) {
    return new DeleteHandler(request, config);
}

DeleteHandler::DeleteHandler(Request& request, Config& config) :
        BaseHandler(config),
        _request(&request) {
}

DeleteHandler::~DeleteHandler() {

}

void DeleteHandler::handle(Response& response) {
    std::string fileName = getResourcePath(_config.getLocationUri(),
                                           _config.getRoot(),
                                           _request->getUri());

    string body;
    if (access(fileName.c_str(), W_OK) != 0) {
        response.setStatusCode("403 Forbidden");
    } else if (removeFile(fileName)) {
        response.setStatusCode("200 OK");
        body = "<html>\n"
               "  <body>\n"
               "    <h1>File deleted.</h1>\n"
               "  </body>\n"
               "</html>";
    } else {
        response.setStatusCode("500 Internal Server Error");
    }
    if (!body.empty()) {
        setBodyToResponse(response, "html", body);
    }

}

bool DeleteHandler::removeFile(const string& fileName) {
    return std::remove(fileName.c_str());
}

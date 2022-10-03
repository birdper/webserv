#include "RequestHandler.hpp"

RequestHandler RequestHandler::getInstance(Request& request, Config& config) {
    return RequestHandler(request, config);
}

RequestHandler::RequestHandler(Request& request, Config& config) :
        _request(request),
        _config(config) {
    _methods["GET"] = GetHandler::getInstance(request, config);
    _methods["POST"] = PostHandler::getInstance(request, config);
    _methods["DELETE"] = DeleteHandler::getInstance(request, config);
}

RequestHandler::~RequestHandler() {
}

Response& RequestHandler::handle() {
    Response& response = *new Response();

    if (isValidResponse(response)) {
        _methods[_request.getMethodString()]->handle(response);
    }
    setErrorPageBodyIfHas(response);
    setStatusCodeAndDescription(response);

    // TODO delete
    Utils::printStatus("request handled");

    return response;
}

bool RequestHandler::isValidResponse(Response& response) {
    if (_request.isBadRequest()) {
        response.setStatusCode("400");
        return false;
    }
    if (!_config.isLocationConfig()) {
        response.setStatusCode("404");
        return false;
    }
    if (!_config.isMethodAllowed(_request.getHttpMethod())) {
        response.setStatusCode("405");
        return false;
    }
    return true;
}

void RequestHandler::setErrorPageBodyIfHas(Response& response) {
    string statusCode = response.getStatusCode();
    int errorCodeNumber = Utils::stringToInt(statusCode, 10);

    if (errorCodeNumber >= 400 && errorCodeNumber <= 599) {
        string errorPageFileName = _config.findCustomErrorPage(statusCode);

        string body = getErrorPage(statusCode);
        if (!body.empty()) {
            string extension = Utils::getExtension(errorPageFileName);
            string contentType = _config.getMimeTypeByExtension(extension);

            response.setBody(body);
            response.addHeader("Content-Type", contentType);
            response.addHeader("Content-Length", std::to_string(body.size()));
        }
    }
}

void RequestHandler::setStatusCodeAndDescription(Response& response) {
    const string& statusCode = response.getStatusCode();
    string description = _config.getDescriptionErrorByCode(statusCode);

    response.setStatusCode(statusCode + " " + description);
}

string RequestHandler::getErrorPage(const string& errorCode) {
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

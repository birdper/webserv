#include "RequestHandler.hpp"

RequestHandler RequestHandler::getInstance(Request& request, Config& config, MimeTypesRepo& mimeTypeRepo) {
    return RequestHandler(request, config, mimeTypeRepo);
}

RequestHandler::RequestHandler(Request& request, Config& config, MimeTypesRepo& mimeTypesRepo) :
        _request(request),
        _config(config),
        _mimeTypesRepo(mimeTypesRepo) {
    _methods["GET"] = GetHandler::getInstance(request, config);
    _methods["POST"] = PostHandler::getInstance(request, config);
    _methods["DELETE"] = DeleteHandler::getInstance(request, config);
}

RequestHandler::~RequestHandler() {
}

Response& RequestHandler::handle() {
    Utils::printStatus("start handle request");

    Response& response = *new Response();

    if (!validate(response)) {
        return response;
    }

    _methods[_request.getMethodString()]->handle(response);
    Utils::printStatus("request handled");

    return response;
}

bool RequestHandler::validate(Response& response) {

    if (_request.isBadRequest()) {
        Utils::printStatus("400 Bad Request");
        return false;
    }
    if (!_config.isLocationConfig()) {
        Utils::printStatus("404 Not Found");
        return false;
    }
    if (!_config.isMethodAllowed(_request.getHttpMethod())) {
        Utils::printStatus("405 Method not Allowed");
        response.setStatusCode("405 Method Not Allowed");
        return false;
    }
    return true;
}

#include "RequestHandler.hpp"

RequestHandler RequestHandler::getInstance(Request& request, Config& config) {
    return RequestHandler(request, config);
}

RequestHandler::RequestHandler(Request& request, Config& config) :
        _request(request),
        _config(config) {
    _methods["GET"] = GetHandler::getInstance(request, config);
    _methods["POST"] = PostHandler::getInstance(request, config);
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
    return true;
}


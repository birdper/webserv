#include "RequestHandler.hpp"

RequestHandler RequestHandler::getInstance(Request& request, Config& config) {
    return RequestHandler(request, config);
}

RequestHandler::RequestHandler(Request& request, Config& config) :
        request(request),
        config(config) {
    _methods["GET"] = GetHandler::getInstance(request, config);
    _methods["POST"] = PostHandler::getInstance(request, config);
}

RequestHandler::~RequestHandler() {
}

Response RequestHandler::handle() {

    Utils::printStatus("start handle request");

    Response response;
    if (!validate(response)) {
        return response;
    }
    response = _methods[request.getMethodString()]->handle(request, config);
    Utils::printStatus("handled method");

    return response;
}

bool RequestHandler::validate(Response& response) {

    if (request.isBadRequest()) {
        Utils::printStatus("400 Bad Request");
        return false;
    }
    if (!config.isLocationConfig()) {
        Utils::printStatus("404 Not Found");
        return false;
    }
    return true;
}


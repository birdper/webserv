#include "RequestHandler.hpp"

RequestHandler::RequestHandler() {
    _methods["GET"] = GetHandler::getInstance();
    _methods["POST"] = PostHandler::getInstance();

}

RequestHandler::~RequestHandler() {
}

Response RequestHandler::handle(Request& request, Config& config) {

    Utils::printStatus("start handle request");

    Utils::printStatus("setupMethods");
    Response response;
//  validate(request, config);
    if (request.isBadRequest()) {
        Utils::printStatus("is bad request");
        return response;
    }
    if (!config.isLocationConfig()) {
        Utils::printStatus("is serv config");
        return response;
    }


    if (request.getHttpMethod() == UNKNOWN_METHOD) {
        return response;
    }
    response = _methods[request.getHttpMethodString()]->handle(request, config);
    Utils::printStatus("handled method");

    return response;
}

bool RequestHandler::validate(Request& request, Config& config) {

    if (!config.isLocationConfig()) {
        return false;
    }
//	_request.getHttpMethod()
    return true;
}

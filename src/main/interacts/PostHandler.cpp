//
// Created by Nicolle Birdperson on 9/14/22.
//

#include "PostHandler.hpp"

BaseHandler* PostHandler::getInstance(Request& request, Config& config) {
    return new PostHandler(request, config);
}

PostHandler::PostHandler(Request& request, Config& config) :
        _request(request),
        _config(config) {
}

PostHandler::~PostHandler() {

}

Response PostHandler::handle() {
    return Response();
}

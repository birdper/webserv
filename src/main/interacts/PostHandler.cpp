//
// Created by Nicolle Birdperson on 9/14/22.
//

#include "PostHandler.hpp"

PostHandler::PostHandler(Request& request, Config& config) :
        BaseHandler() {
}

PostHandler::~PostHandler() {

}

Response PostHandler::handle(Request& request, Config& config) {

    return Response();
}

BaseHandler* PostHandler::getInstance() {
    return new PostHandler();
}

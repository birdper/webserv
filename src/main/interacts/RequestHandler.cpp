#include "RequestHandler.hpp"

/*
RequestHandler::RequestHandler(Config& config) :
		config(config) {}
*/

Response RequestHandler::handle(Request& request, Config& config) {

	Response response;
	if (request.isBadRequest())
		return response;

	switch (request.getMethod()) {
		case GET:
			config.getUri();
//			handleGet(request);
			break;
		case PUT:
//			handleGet(client, request);
			break;
		case POST:
//			handleGet(client, request);
			break;
		case DELETE:
//			handleGet(client, request);
			break;
		default:
//			sendStatusResponse(client, Status(NOT_IMPLEMENTED));
			break;
	}
	return response;
}

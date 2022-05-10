
#include "RequestHandler.hpp"

Response RequestHandler::handle(Request& request) {
	switch (request.getMethod()) {
		case GET:
//			handleGet(client, request);
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
	return Response();
}

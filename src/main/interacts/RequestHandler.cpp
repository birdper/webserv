#include "RequestHandler.hpp"

RequestHandler RequestHandler::getInstance(Request& request, Config& config) {
	return RequestHandler(request, config);
}

RequestHandler::RequestHandler(Request& request, Config& config) :
		_request(request),
		_config(config) {
	_methods["GET"] = GetHandler::getInstance(request, config);
	_methods["POST"] = PostHandler::getInstance(request, config);
	_methods["PUT"] = PostHandler::getInstance(request, config);
	_methods["DELETE"] = DeleteHandler::getInstance(request, config);
}

RequestHandler::~RequestHandler() {
}

Response& RequestHandler::handle() {
	Response& response = *new Response();

	if (isValidResponse(response)) {
		_methods[_request.getMethodString()]->handle(response);
	}
	if (isCgiRequest()) {
		handleCGI(response);
	}
	if (isErrorResponse(response)) {
		setErrorPageBody(response);
	}
	response.addHeader("Connection", "close");
	setStatusCodeAndDescription(response);

	return response;
}

bool RequestHandler::isCgiRequest() {
	return !_config.getPathCGI().empty() && Utils::getExtension(_request.getUri()) == _config.getExtensionCGI();
}

void RequestHandler::handleCGI(Response& response) const {
	CGIHandler cgiHandler;
	response.setStatusCode(cgiHandler.handle(_config.getPathCGI(), response.getResource(), &_request));
	string body = cgiHandler.getBody();
	response.setBody(body);
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
	if (Utils::stringToInt(_config.getClientMaxBodySize(), 10) < Utils::stringToInt(_request.findHeaderValue("Content-Length"), 10)) {
		response.setStatusCode("413");
		return false;
	}
	return true;
}

bool RequestHandler::isErrorResponse(Response& response) {
	int errorCodeNumber = Utils::stringToInt(response.getStatusCode(), 10);
	return errorCodeNumber >= 400 && errorCodeNumber <= 599;
}

void RequestHandler::setErrorPageBody(Response& response) {
	string statusCode = response.getStatusCode();

	string errorPageFileName = _config.findCustomErrorPage(statusCode);

	string body = getErrorPageBody(statusCode);
	if (!body.empty()) {
		string extension = Utils::getExtension(errorPageFileName);
		string contentType = _config.getMimeTypeByExtension(extension);

		response.setBody(body);
		response.addHeader("Content-Type", contentType);
		response.addHeader("Content-Length", std::to_string(body.size()));
	}
}

void RequestHandler::setStatusCodeAndDescription(Response& response) {
	const string& statusCode = response.getStatusCode();
	string description = _config.getDescriptionByCode(statusCode);

	response.setStatusCode(statusCode + " " + description);
}

string RequestHandler::getErrorPageBody(const string& errorCode) {
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

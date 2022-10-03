#include "RequestParser.hpp"

void RequestParser::parse(Request& request, Client& client) const {
	const string& requestBuffer = client.getBuffer();
	size_t borderLinePosition = requestBuffer.find(HTTP_BORDER_LINE);

	if (borderLinePosition != string::npos && isHasHeaders(request)) {
		string headersString = requestBuffer.substr(0, borderLinePosition);
		std::vector<string> headerLines = Utils::split(headersString, END_OF_LINE);

		parseStartLine(headerLines[0], request);
		parseHeaders(headerLines, request);

		client.setBuffer(requestBuffer.substr(borderLinePosition + HTTP_BORDER_LINE.length()));

		if (request.getHttpMethod() != POST && request.getHttpMethod() != PUT) {
			client.setIsReadyRequest(true);
			client.setBuffer("");
		}
		// TODO delete debug print
		printRequest(headerLines);
	}
	if (request.getHttpMethod() == POST || request.getHttpMethod() == PUT) {

		string contentLength = request.findHeaderValue("Content-Length");
		if (request.findHeaderValue("Transfer-Encoding") == "chunked") {
			parseChunked(request, client);
		} else if (!contentLength.empty()) {
			parseBodyContent(request, client.getBuffer(), client);
		}
	}
	/*    try {

    } catch (ParseRequestException& ex) {
        Utils::printStatus(ex.what());
        request.setBadStatus();
    }
    return request;*/
}

bool RequestParser::isHasHeaders(const Request& request) const {
	return request.getHeaders().empty();
}

void RequestParser::parseStartLine(const string& line, Request& request) const {
	std::vector<string> startLineTokens = Utils::split(line, " ");

	if (startLineTokens.size() != 3) {
		throw ParseRequestException("Incorrect format Start Line '" + line + "'");
	}

	parseMethod(request, startLineTokens[0]);
	parseUri(request, startLineTokens[1]);
	parseHttpVersion(request, startLineTokens[2]);
}

void RequestParser::parseMethod(Request& request, const string& httpMethod) const {
	request.setMethodString(httpMethod);
	if (httpMethod == "GET") {
		request.setMethodEnum(GET);
	} else if (httpMethod == "PUT") {
		request.setMethodEnum(PUT);
	} else if (httpMethod == "POST") {
		request.setMethodEnum(POST);
	} else if (httpMethod == "DELETE") {
		request.setMethodEnum(DELETE);
	} else {
		throw ParseRequestException("Wrong HTTP Method '" + httpMethod + "'");
	}
}

void RequestParser::parseUri(Request& request, const string& uri) const {
	if (uri[0] == '/') {
		request.setUri(uri);
	} else {
		throw ParseRequestException("Incorrect uri '" + uri + "'");
	}
}

void RequestParser::parseHttpVersion(Request& request, const string& httpVersion) const {
	if (httpVersion == "HTTP/1.1") {
		request.setHttpVersion(httpVersion);
	} else {
		throw ParseRequestException("Incorrect Version Protocol '" + httpVersion + "'");
	}
}

void RequestParser::parseHeaders(std::vector<string>& headerLines, Request& request) const {
	std::map<string, string>* headers = new std::map<string, string>();

	for (int i = 1; i < headerLines.size(); ++i) {
		size_t separatorPosition = headerLines[i].find(':');
		string header = headerLines[i].substr(0, separatorPosition);
		string value = headerLines[i].substr(separatorPosition + 2);
		(*headers)[header] = value;
	}
	request.setHeaders(*headers);

	if (request.findHeaderValue("Host").empty()) {
		throw ParseRequestException("Not found header 'Host'");
	}
}

void RequestParser::parseChunked(Request& request, Client& client) const {

	const string endOfChunkedRequest = "0\r\n\r\n";

	const string& chunks = client.getBuffer();
	size_t chunkedRequestEnd = chunks.find(endOfChunkedRequest);

	if (chunkedRequestEnd != std::string::npos) {
		if (chunks.length() != chunkedRequestEnd + endOfChunkedRequest.length()) {
			throw ParseRequestException(
					"requestBuffer.length() != chunkedRequestEndPos + END_OF_CHUNKED_REQUEST.length()");
		}
		size_t sizeEndPos = chunks.find(END_OF_LINE);
		size_t chunkSize = Utils::stringToInt(chunks.substr(0, sizeEndPos), 16);

		while (chunkSize > 0) {
			sizeEndPos += 2;
			string chunk = chunks.substr(sizeEndPos, chunkSize);

			request.appendBody(chunk);

			size_t chunkEndPos = chunkSize + sizeEndPos + 2;
			sizeEndPos = chunks.find(END_OF_LINE, chunkEndPos);
			chunkSize = Utils::stringToInt(chunks.substr(chunkEndPos, sizeEndPos - chunkEndPos), 16);
		}
		request.setBuffer("");
//		request.setFileName(getFileName(request.getUri()));
//		request.setIsReady(true);
		client.setIsReadyRequest(true);
	}
}

void RequestParser::parseBodyContent(Request& request, const string& buffer, Client& client) const {
//    TODO перенести в PostHandler
//    string fileName = Utils::getFileName(request.getLocationUri());
//    request.setFileName(fileName);

	size_t contentLength = Utils::stringToInt(request.findHeaderValue("Content-Length"), 10);
//    if (request.getBody().size() > contentLength) {
//        throw ParseRequestException("400 BAD REQUEST! RECV size > then MUST BE");
//    }
	request.appendBody(buffer);

	if (contentLength == request.getBody().size()) {
		client.setIsReadyRequest(true);
		client.setBuffer("");
	}
}

void RequestParser::printRequest(const std::vector<string>& headerLines) const {
	cout << "\n=========REQUEST=========" << endl;
	for (int i = 0; i < headerLines.size(); ++i) {
		std::cout << headerLines[i] << std::endl;
	}
	cout << "=========END_REQUEST============" << endl << endl;
}

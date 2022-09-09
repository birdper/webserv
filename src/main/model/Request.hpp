#pragma once

#include <iostream>
#include <map>
#include "HttpMethod.hpp"

class Request {
private:
	bool isBadStatus;
	HttpMethod method;
	std::string uri;
	std::string httpVersion;
	std::map<std::string, std::string> headers;

public:
	Request();
	Request(const Request&);
	Request& operator=(const Request&);

	bool isBadRequest() const;
	void setBadStatus();

	HttpMethod getMethod() const;
	void setMethod(HttpMethod method);

	void setUri(const std::string& requestUri);
	const std::string& getUri() const;

	const std::string& getHttpVersion() const;
	void setHttpVersion(const std::string& version);

	const std::map<std::string, std::string>& getHeaders() const;
	void setHeaders(const std::map<std::string, std::string>& headers);

	const std::string& findHeaderByName(const std::string& headerName) const;

};
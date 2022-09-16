#pragma once

#include <iostream>
#include <map>
#include "HttpMethod.hpp"
#include "HttpMessage.hpp"

class Request : public HttpMessage {
private:
	HttpMethod _methodEnum;
    std::string _methodString;
	std::string uri;

	bool isBadStatus;

public:
	Request();
	Request(const Request&);
	Request& operator=(const Request&);
    virtual ~Request();

    bool isBadRequest() const;
	void setBadStatus();

	HttpMethod getHttpMethod() const;
	const std::string& getMethodString() const;
	void setMethodEnum(HttpMethod method);
    void setMethodString(const std::string& httpMethodString);

    void setUri(const std::string& requestUri);
	const std::string& getUri() const;
};

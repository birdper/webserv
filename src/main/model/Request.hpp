#pragma once

#include <iostream>
#include <map>
#include "HttpMethod.hpp"
#include "HttpMessage.hpp"
#include "usings.hpp"

class Request : public HttpMessage {
private:
	HttpMethod _methodEnum;
    string _methodString;
	string uri;
    string _body;
    string _buffer;

    bool isBadStatus;

public:
    const string& getBuffer() const;
    void setBuffer(const string& buffer);
    Request();
	Request(const Request&);
	Request& operator=(const Request&);
    virtual ~Request();

    bool isBadRequest() const;
	void setBadStatus();

	HttpMethod getHttpMethod() const;
	const string& getMethodString() const;
	void setMethodEnum(HttpMethod method);
    void setMethodString(const string& httpMethodString);

    void setUri(const string& requestUri);
    void appendBody(const string& buffer);
	const string& getUri() const;
    const string& getBody() const;
    void setBody(const string& body);
};

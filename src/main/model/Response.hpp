#pragma once

#include <iostream>
#include <sstream>

#include "HttpMessage.hpp"
#include "Constants.hpp"
#include "Utils.hpp"


class Response : public HttpMessage {
private:
	std::string statusCode;

    // Возможно стоит перенести боди в клиента
    std::string body;

public:
    string serialize();
    virtual ~Response();

    const std::string& getStatusCode() const;
	void setStatusCode(const std::string& statusCode);

    const std::string& getBody() const;
	void setBody(const std::string& body);

private:
    string& parseToHeaders();

};
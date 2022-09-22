#pragma once

#include <iostream>
#include <sstream>

#include "HttpMessage.hpp"
#include "Constants.hpp"
#include "Utils.hpp"


class Response : public HttpMessage {
private:
	std::string reason;
	std::string statusCode;

	// Возможно стоит перенести боди в клиента
	std::string body;

public:
    friend std::ostream& operator<<(std::ostream& os, const Response& response);
    std::string& serialize();
    virtual ~Response();

    const std::string& getReason() const;
	void setReason(const std::string& reason);
	const std::string& getStatusCode() const;
	void setStatusCode(const std::string& statusCode);
	const std::string& getBody() const;
	void setBody(const std::string& body);

    string& parseToHeaders();
};
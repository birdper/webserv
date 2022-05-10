#pragma once

#include <iostream>
#include "HttpMethod.hpp"

class Request {

private:
	HttpMethod method;
	std::string requestUri;

public:
	HttpMethod getMethod() const;
	const std::string& getRequestUri() const;
};
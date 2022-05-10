
#include "Request.hpp"

HttpMethod Request::getMethod() const {
	return method;
}

const std::string& Request::getRequestUri() const {
	return requestUri;
}



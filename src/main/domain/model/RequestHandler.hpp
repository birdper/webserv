#pragma once

#include <iostream>
#include "WebClient.hpp"

class RequestHandler {
public:
	virtual void getResponse(WebClient *client) = 0;
};
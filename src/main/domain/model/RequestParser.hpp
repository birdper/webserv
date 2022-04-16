#pragma once

#include <iostream>
#include "WebClient.hpp"

class RequestParser {

public:
	virtual void handleRequest(WebClient* client) = 0;

};
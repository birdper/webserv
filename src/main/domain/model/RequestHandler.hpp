#pragma once

#include <iostream>
#include "Client.hpp"

class RequestHandler {
public:
	virtual void getResponse(Client *client) = 0;
};
#pragma once

#include <iostream>
#include "Client.hpp"

class RequestHandler {
public:
	Response handle(Request& request);
};
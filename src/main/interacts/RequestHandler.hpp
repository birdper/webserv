#pragma once

#include <iostream>
#include "Client.hpp"
#include "Config.hpp"

class RequestHandler {
private:
	Config &config;

public:
	RequestHandler(Config& config);
	Response handle(Request& request);
};
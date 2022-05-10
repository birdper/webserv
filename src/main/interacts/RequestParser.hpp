#pragma once

#include <iostream>
#include <sstream>

#include "Client.hpp"

class RequestParser {

public:
	Request* parse(Client* client, std::string& data);

};
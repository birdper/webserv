#pragma once

#include <iostream>
#include "Client.hpp"

class RequestParser {

public:
	virtual void parse(Client* client) = 0;

};
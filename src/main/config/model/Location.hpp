//
// Created by nbirdper on 28.01.2022.
//

#pragma once

#include "VirtualServer.hpp"
#include "Parameters.hpp"

class VirtualServer;

class Location {

private:
//	VirtualServer* parentServer;
	Parameters _params;
	std::string _uri;

public:
	Location(const std::string& uri,
			 Parameters* parentParams);

	Parameters* getParameters();
	std::string getUri() const;
};
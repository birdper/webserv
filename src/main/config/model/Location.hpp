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
	Parameters params;

public:
	Location(const std::string& uri,
			 Parameters* parentParams);

	Parameters* getParameters();
	const std::string& getUri() const;
};
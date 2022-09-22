//
// Created by nbirdper on 28.01.2022.
//

#pragma once

#include "VirtualServer.hpp"
#include "Parameters.hpp"

class VirtualServer;

class Location {
private:
	Parameters _params;

public:
    Location(const std::string& uri,
             const Parameters& parentParams);
    Location(Location& location);
    Location& operator=(const Location& other);

	Parameters& getParameters();
    std::string getUri() const;
};
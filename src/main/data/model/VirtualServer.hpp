//
// Created by nbirdper on 28.01.2022.
//

#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "Location.hpp"
#include "DefaultParameters.hpp"

class Location;

class VirtualServer {
private:
	std::string ip;
	std::string port;
	std::string host;
	std::vector<std::string> serverNames;

	Parameters params;
	std::vector<Location*> locations;

public:
//	TODO времмено добавлен пустой кноструктор
	VirtualServer();
	VirtualServer(const std::string& host, const std::string& port);
	VirtualServer(Parameters *defaultParameters);

	const std::string& getHost() const;
	void setHost(const std::string& host);
	Parameters* getParameters();
	const std::vector<Location*>& getLocations() const;
	const std::vector<std::string>& getServerNames() const;
	void addLocation(Location* location);
	void addServerNames(const std::vector<std::string>& names);
	bool isContainServerName(const std::string& serverName) const;
};
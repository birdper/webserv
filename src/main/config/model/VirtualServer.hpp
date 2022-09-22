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
    int port;
	std::string ip;
	std::string host;
	std::vector<std::string> serverNames;

	Parameters _params;
	std::vector<Location*> _locations;

public:
	VirtualServer();
//	VirtualServer(Parameters *defaultParameters);

    const std::string &getIp() const;
    void setIp(const std::string &ip);
    int getPort() const;
    void setPort(int port);
	const std::string& getHost() const;
	void setHost(const std::string& host);

	bool isContainServerName(const std::string& serverName) const;
	void addServerNames(const std::vector<std::string>& names);
	const std::vector<std::string>& getServerNames() const;
	Parameters& getParameters();
	std::vector<Location*> getLocations() const;
	void addLocation(Location* location);
};
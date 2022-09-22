//
// Created by nbirdper on 28.01.2022.
//

#include "VirtualServer.hpp"

VirtualServer::VirtualServer() {}

/*
    VirtualServer::VirtualServer(Parameters* defaultParameters) {
	ip = defaultParameters->ip;
	port = std::stoi(defaultParameters->port);

	params.root = defaultParameters->root;
	params.pathCGI = defaultParameters->pathCGI;
	params.extensionCGI = defaultParameters->extensionCGI;
	params.clientMaxBodySize = defaultParameters->clientMaxBodySize;
	params.indexFiles = defaultParameters->indexFiles;
}
*/

Parameters& VirtualServer::getParameters() {
	return _params;
}

const std::vector<std::string>& VirtualServer::getServerNames() const {
	return serverNames;
}

std::vector<Location*> VirtualServer::getLocations() const {
	return _locations;
}

void VirtualServer::addLocation(Location* location) {
	_locations.push_back(location);
}

void VirtualServer::addServerNames(const std::vector<std::string>& names) {
	serverNames = names;
}

const std::string& VirtualServer::getHost() const {
	return host;
}

void VirtualServer::setHost(const std::string& host) {
	VirtualServer::host = host;
}

bool VirtualServer::isContainServerName(const std::string& serverName) const {

	const std::vector<std::string>::const_iterator it = std::find(serverNames.begin(),
																  serverNames.end(), serverName);
	if (it == serverNames.end())
		return false;
	return true;
}

const std::string &VirtualServer::getIp() const {
    return ip;
}

void VirtualServer::setIp(const std::string &ip) {
    VirtualServer::ip = ip;
}

int VirtualServer::getPort() const {
    return port;
}

void VirtualServer::setPort(int port) {
    VirtualServer::port = port;
}

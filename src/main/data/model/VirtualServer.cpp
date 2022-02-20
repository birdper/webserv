//
// Created by nbirdper on 28.01.2022.
//

#include "VirtualServer.hpp"


VirtualServer::VirtualServer() {}

VirtualServer::VirtualServer(const std::string& host, const std::string& port) :
		ip(host),
		port(port) {}

VirtualServer::VirtualServer(Parameters* defaultParameters) {
	ip = defaultParameters->ip;
	port = defaultParameters->port;

	params.root = defaultParameters->root;
	params.pathCGI = defaultParameters->pathCGI;
	params.extensionCGI = defaultParameters->extensionCGI;
	params.clientMaxBody = defaultParameters->clientMaxBody;
	params.indexFiles = defaultParameters->indexFiles;
}

Parameters* VirtualServer::getParameters() {
	return &params;
}

const std::vector<std::string>& VirtualServer::getServerNames() const {
	return serverNames;
}

const std::vector<Location*>& VirtualServer::getLocations() const {
	return locations;
}

void VirtualServer::addLocation(Location* location) {
	locations.push_back(location);
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

bool
VirtualServer::isContainServerName(const std::string& serverName) const {

	const std::vector<std::string>::const_iterator it = std::find(
			serverNames.begin(),
			serverNames.end(),
			serverName);
	if (it == serverNames.end())
		return false;
	return true;
}
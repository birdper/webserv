//
// Created by nbirdper on 06.02.2022.
//

#include "SettingsStorage.hpp"

void SettingsStorage::setVirtualServers(MapHostVectorVirtualServers* virtualServers) {
	SettingsStorage::virtualServers = *virtualServers;
}

void SettingsStorage::addVirtualServerByHost(const std::string& host,
											 VirtualServer* server) {
	server->setHost(host);
	MapHostVectorVirtualServers::iterator it = virtualServers.find(host);
	if (it == virtualServers.end()) {
		virtualServers[host].push_back(server);
	} else {
		it->second.push_back(server);
	}
}

void SettingsStorage::addLocation(const std::string& uri,
								  const VirtualServer* server) {

}

VirtualServer& SettingsStorage::findVirtualServer(const std::string& host,
												  const std::string& serverName) const {

/*
	MapHostVectorVirtualServers::const_iterator it = virtualServers.find(host);
	if (it == virtualServers.end()) {
		return nullptr;
	}

	const std::vector<VirtualServer*>& vectorVirtualServers = it->second;
	VirtualServer* virtualServer = vectorVirtualServers[0];

	if (!serverName.empty()) {
		std::cout << "find by server_name: " << serverName << std::endl;
		for (size_t i = 0; i < vectorVirtualServers.size(); ++i) {
			if (vectorVirtualServers[i]->isContainServerName(serverName)) {
				virtualServer = vectorVirtualServers[i];
				break;
			}
		}
	}
 */
	const std::vector<VirtualServer*>* servers = getVirtualServersByHost(host);
	return getVirtualServerByServerNameOrDefault(*servers, serverName);
}


const std::vector<VirtualServer*>*
SettingsStorage::getVirtualServersByHost(const std::string& host) const {
	MapHostVectorVirtualServers::const_iterator it = virtualServers.find(host);
	if (it == virtualServers.end()) {
		return nullptr;
	}
	return &it->second;
}


VirtualServer& SettingsStorage::getVirtualServerByServerNameOrDefault(
		const std::vector<VirtualServer*>& servers,
		const std::string& serverName) const {

	VirtualServer* virtualServer = servers[0];

	if (!serverName.empty()) {
		for (size_t i = 0; i < servers.size(); ++i) {
			if (servers[i]->isContainServerName(serverName)) {
				virtualServer = servers[i];
				break;
			}
		}
	}
	return virtualServer;
}


const MapHostVectorVirtualServers& SettingsStorage::getVirtualServers() const {
	return virtualServers;
}

/*Location* SettingsStorage::findLocationByUri(std::vector<Location*>* locations,
											 const std::string& uriRequest) const {
	Location* currentLocation;
	Location* mostLengthMatch;
	size_t maxLength = 0;
	for (int i = 0; i < locations->size(); ++i) {
		currentLocation = (*locations)[i];
		std::string uriLocation = currentLocation->getParameters()->uri;
//		TODO implement searching location by uri
		size_t lengthMatch = getLengthMatch(uriLocation, uriRequest);
		if (lengthMatch > maxLength) {
			maxLength = lengthMatch;
			mostLengthMatch = currentLocation;
		}
	}
	if (maxLength)
		return mostLengthMatch;
	return nullptr;
}*/

/*size_t SettingsStorage::getLengthMatch(const std::string& uriLocation,
									   const std::string& uriRequest) const {
	size_t length = 0;
	for (int i = 0; i < uriLocation.length(); ++i) {
		if (uriLocation[i] == uriRequest[i])
			++length;
	}
	return length;
}*/

/*
bool
SettingsStorage::containServerName(const VirtualServer* virtualServer,
									 const std::string& serverName) const {

	const std::vector<std::string>& serverNames = virtualServer->getServerNames();
	const std::vector<std::string>::const_iterator it =
			std::find(serverNames.begin(),
					  serverNames.end(),
					  serverName);
	if (it == serverNames.end())
		return false;
	return true;
}*/
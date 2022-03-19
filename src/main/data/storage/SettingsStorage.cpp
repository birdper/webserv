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

	const std::vector<VirtualServer*>* servers = getVirtualServersByHost(host);
	return getVirtualServerByNameOrDefault(*servers, serverName);
}

const std::vector<VirtualServer*>*
SettingsStorage::getVirtualServersByHost(const std::string& host) const {
	MapHostVectorVirtualServers::const_iterator it = virtualServers.find(host);
	if (it == virtualServers.end()) {
		return nullptr;
	}
	return &it->second;
}

VirtualServer& SettingsStorage::getVirtualServerByNameOrDefault(
		const std::vector<VirtualServer*>& servers,
		const std::string& serverName) const {

	VirtualServer* foundVirtualServer = servers[0];

	if (!serverName.empty()) {
		for (size_t i = 0; i < servers.size(); ++i) {
			if (servers[i]->isContainServerName(serverName)) {
				foundVirtualServer = servers[i];
				break;
			}
		}
	}
	return *foundVirtualServer;
}

const MapHostVectorVirtualServers& SettingsStorage::getVirtualServers() const {
	return virtualServers;
}
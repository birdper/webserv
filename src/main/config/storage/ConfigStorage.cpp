//
// Created by nbirdper on 06.02.2022.
//

#include "ConfigStorage.hpp"

void ConfigStorage::addVirtualServerByHost(const std::string& port,
										   VirtualServer* server) {
	MapHostVectorVirtualServers::iterator it = virtualServers.find(port);
	if (it == virtualServers.end()) {
		virtualServers[port].push_back(server);
	} else {
		it->second.push_back(server);
	}
}

const MapHostVectorVirtualServers& ConfigStorage::getVirtualServers() const {
	return virtualServers;
}

VirtualServer& ConfigStorage::findVirtualServer(const std::string& host,
												const std::string& serverName) {

	const std::vector<VirtualServer*> servers = getVirtualServersByPort(host);
	return getVirtualServerByNameOrDefault(servers, serverName);
}

std::vector<VirtualServer*>
ConfigStorage::getVirtualServersByPort(const std::string& port) {
	MapHostVectorVirtualServers::iterator it = virtualServers.find(port);

	if (it == virtualServers.end()) {
		return std::vector<VirtualServer*>();
	}
	return it->second;
}

VirtualServer& ConfigStorage::getVirtualServerByNameOrDefault(const std::vector<VirtualServer*>& servers,
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

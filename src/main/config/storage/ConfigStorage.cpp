//
// Created by nbirdper on 06.02.2022.
//

#include "ConfigStorage.hpp"

void ConfigStorage::setVirtualServers(MapHostVectorVirtualServers* virtualServers) {
	ConfigStorage::virtualServers = *virtualServers;
}

void ConfigStorage::addVirtualServerByHost(const std::string& host,
										   VirtualServer* server) {
//	server->setHost(host);
	MapHostVectorVirtualServers::iterator it = virtualServers.find(host);
	if (it == virtualServers.end()) {
		virtualServers[host].push_back(server);
	} else {
		it->second.push_back(server);
	}
}

void ConfigStorage::addLocation(const std::string& uri,
								const VirtualServer* server) {

}

VirtualServer& ConfigStorage::findVirtualServer(const std::string& host,
												const std::string& serverName) const {

	const std::vector<VirtualServer*>* servers = getVirtualServersByHost(host);
	return getVirtualServerByNameOrDefault(*servers, serverName);
}

const std::vector<VirtualServer*>*
ConfigStorage::getVirtualServersByHost(const std::string& host) const {
	MapHostVectorVirtualServers::const_iterator it = virtualServers.find(host);
	if (it == virtualServers.end()) {
		return nullptr;
	}
	return &it->second;
}

VirtualServer& ConfigStorage::getVirtualServerByNameOrDefault(
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

const MapHostVectorVirtualServers& ConfigStorage::getVirtualServers() const {
	return virtualServers;
}

std::vector<VirtualServer *> ConfigStorage::getVirtualServs() {
	std::vector<VirtualServer*> values;
    std::cout << "getVirtServ()" << std::endl;

    int i = 0;
	for (MapHostVectorVirtualServers::iterator it = virtualServers.begin(); it != virtualServers.end(); ++it) {
        std::cout << "virt iter " << i << std::endl;
        ++i;
		values.push_back(it->second[0]);
	}
    return values;
}
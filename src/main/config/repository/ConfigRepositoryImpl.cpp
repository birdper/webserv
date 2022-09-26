//
// Created by nbirdper on 20.02.2022.
//

#include "ConfigRepositoryImpl.hpp"

ConfigRepositoryImpl::ConfigRepositoryImpl(ConfigStorage* storage) :
		storage(storage) {
}

std::vector<std::pair<std::string, int> > ConfigRepositoryImpl::getHostsForBind() const {
	std::vector<std::pair<std::string, int> > hosts;
	MapHostVectorVirtualServers allServers = storage->getVirtualServers();
	for (MapHostVectorVirtualServers::iterator it = allServers.begin(); it != allServers.end(); ++it) {
		VirtualServer* virtualServer = it->second[0];
		hosts.push_back(std::make_pair(virtualServer->getIp(), virtualServer->getPort()));
	}
	return hosts;
}

std::vector<VirtualServer*> ConfigRepositoryImpl::getVirtualServersForBind() const {
	std::vector<VirtualServer*> hosts;
	MapHostVectorVirtualServers allServers = storage->getVirtualServers();
	for (MapHostVectorVirtualServers::iterator it = allServers.begin(); it != allServers.end(); ++it) {
		hosts.push_back(it->second[0]);
	}
	return hosts;
}

VirtualServer&
ConfigRepositoryImpl::getServerConfig(const std::string& ip,
                                      const std::string& port,
                                      const std::string& serverName) const {

	std::vector<VirtualServer*> serversByPort = storage->getVirtualServersByPort(port);

	std::vector<VirtualServer*> serversSameIp;
	for (int i = 0; i < serversByPort.size(); ++i) {
		if (serversByPort[i]->getIp() == ip) {
			serversSameIp.push_back(serversByPort[i]);
		}
	}
	if (!serversSameIp.empty() && !serverName.empty()) {
		for (int i = 0; i < serversSameIp.size(); ++i) {
			if (serversSameIp[i]->isContainServerName(serverName))
				return *serversSameIp[i];
		}
	}
	return *serversSameIp[0];
}

Config* ConfigRepositoryImpl::findLocationConfigByUri(const VirtualServer& virtualServer,
                                                      const string& requestUri) const {
	std::vector<Location*> locations = virtualServer.getLocations();

	Location* foundLocation = nullptr;
	size_t maxLength = 0;

	for (int i = 0; i < locations.size(); ++i) {
		Location* currentLocation = locations[i];
		string locationUri = locations[i]->getUri();

		if (requestUri.compare(0, locationUri.length(), locationUri) == 0) {
			size_t curLen = locationUri.length();
			if (curLen > maxLength) {
				maxLength = curLen;
				foundLocation = currentLocation;
			}
		}
	}

	Config* config = nullptr;
	if (foundLocation != nullptr) {
		config = new Config(foundLocation->getParameters(), true);
	}
	return config;
}

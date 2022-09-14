//
// Created by nbirdper on 20.02.2022.
//

#include "ConfigRepositoryImpl.hpp"

ConfigRepositoryImpl::ConfigRepositoryImpl(ConfigStorage* storage) :
        storage(storage) {}

//	TODO fill description method
/**
 * @param requestUri
 * @param host
 * @param serverName may be empty
 * @return Config instance or nullptr
 */
Config* ConfigRepositoryImpl::getConfig(const std::string& requestUri, const std::string& host,
                                        const std::string& serverName) const {

    VirtualServer server = storage->findVirtualServer(host, serverName);
    std::vector<Location*> locations = server.getLocations();
    Location* location = findLocationByUri(locations, requestUri);

    return location == nullptr ? nullptr
                               : new Config(location->getParameters());
}

Location* ConfigRepositoryImpl::findLocationByUri(std::vector<Location*>& locations,
                                                  const std::string& requestUri) const {
    Location* currentLocation;
    Location* mostLengthMatch;
    size_t maxLength = 0;
    for (int i = 0; i < locations.size(); ++i) {
        currentLocation = locations[i];
        std::string locationUri = currentLocation->getParameters()->uri;
//		TODO implement location search by uri
        size_t lengthMatch = getLengthMatch(locationUri, requestUri);
        if (lengthMatch > maxLength) {
            maxLength = lengthMatch;
            mostLengthMatch = currentLocation;
        }
    }
    if (maxLength)
        return mostLengthMatch;
    return nullptr;
}

size_t ConfigRepositoryImpl::getLengthMatch(const std::string& locationUri,
                                            const std::string& requestUri) const {
    size_t length = 0;
    for (int i = 0; i < locationUri.length(); ++i) {
        if (locationUri[i] == requestUri[i])
            ++length;
    }
    return length;
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

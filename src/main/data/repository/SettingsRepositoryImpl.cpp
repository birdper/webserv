//
// Created by nbirdper on 20.02.2022.
//

#include "SettingsRepositoryImpl.hpp"

SettingsRepositoryImpl::SettingsRepositoryImpl(SettingsStorage* storage) :
		storage(storage) {}

std::vector<std::string> SettingsRepositoryImpl::getHostsVirtualServers() const {
	std::vector<std::string> hosts;
	MapHostVectorVirtualServers map = storage->getVirtualServers();
	MapHostVectorVirtualServers::iterator it = map.begin();
	for (; it != map.end(); ++it) {
		hosts.push_back(it->first);
	}
	return hosts;
}


//	TODO fill description method
/**
 * @param uriRequest
 * @param host
 * @param serverName may be empty
 * @return Config instance or nullptr
 */
Config* SettingsRepositoryImpl::getConfig(const std::string& uriRequest, const std::string& host,
										  const std::string& serverName) const {

	VirtualServer server = storage->findVirtualServer(host, serverName);
	std::vector<Location*> locations = server.getLocations();
	Location* location = findLocationByUri(locations, uriRequest);

	return location == nullptr ? nullptr
							   : new Config(location->getParameters());
}

Location* SettingsRepositoryImpl::findLocationByUri(std::vector<Location*>& locations,
													const std::string& uriRequest) const {
	Location* currentLocation;
	Location* mostLengthMatch;
	size_t maxLength = 0;
	for (int i = 0; i < locations.size(); ++i) {
		currentLocation = locations[i];
		std::string uriLocation = currentLocation->getParameters()->uri;
//		TODO implement location search by uri
		size_t lengthMatch = getLengthMatch(uriLocation, uriRequest);
		if (lengthMatch > maxLength) {
			maxLength = lengthMatch;
			mostLengthMatch = currentLocation;
		}
	}
	if (maxLength)
		return mostLengthMatch;
	return nullptr;
}

size_t SettingsRepositoryImpl::getLengthMatch(const std::string& uriLocation,
											  const std::string& uriRequest) const {
	size_t length = 0;
	for (int i = 0; i < uriLocation.length(); ++i) {
		if (uriLocation[i] == uriRequest[i])
			++length;
	}
	return length;
}
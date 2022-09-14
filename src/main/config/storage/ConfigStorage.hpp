//
// Created by nbirdper on 06.02.2022.
//

#pragma once

#include "VirtualServer.hpp"
#include "SettingTypes.hpp"

class ConfigStorage {

private:
	MapHostVectorVirtualServers virtualServers;

public:
	void addVirtualServerByHost(const std::string& port,
								VirtualServer* server);
	const MapHostVectorVirtualServers& getVirtualServers() const;
	VirtualServer& findVirtualServer(const std::string& host,
									 const std::string& serverName) const;

private:
	const std::vector<VirtualServer*>* getVirtualServersByHost(
			const std::string& host) const;

	VirtualServer&
	getVirtualServerByNameOrDefault(const std::vector<VirtualServer*>& servers,
									const std::string& serverName) const;
};
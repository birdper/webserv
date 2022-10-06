//
// Created by nbirdper on 29.01.2022.
//

#include "DefaultParameters.hpp"

DefaultParameters::DefaultParameters() {
	params.root = "";
	params.pathCGI = "";
	params.extensionCGI = "";
	params.clientMaxBodySize = "10m";
	params.indexNameFiles.push_back("index");
	params.indexNameFiles.push_back("index.html");
}
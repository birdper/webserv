//
// Created by nbirdper on 29.01.2022.
//

#include "DefaultParameters.hpp"

DefaultParameters::DefaultParameters() {
//	TODO установить текущий каталог
	params.root = "";
//	TODO установить каталог
	params.pathCGI = "";
	params.extensionCGI = ".php";
	params.clientMaxBodySize = "10m";
	params.indexNameFiles.push_back("index");
	params.indexNameFiles.push_back("index.html");
}
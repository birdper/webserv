//
// Created by nbirdper on 29.01.2022.
//

#include "DefaultParameters.hpp"

DefaultParameters::DefaultParameters() {
	params.ip = "0.0.0.0";
	params.port = "8080";
//	TODO установить текущий каталог
	params.root = "";
//	TODO установить каталог
	params.pathCGI = "";
	params.extensionCGI = ".php";
	params.clientMaxBody = "10m";
	params.indexFiles.push_back("index");
	params.indexFiles.push_back("index.html");
}
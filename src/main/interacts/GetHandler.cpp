
#include "GetHandler.hpp"


BaseHandler* GetHandler::getInstance(Request& request, Config& config) {
	return new GetHandler(request, config);
}

GetHandler::GetHandler(Request& request, Config& config) :
        BaseHandler(request, config) {
}

GetHandler::~GetHandler() {
}

void GetHandler::handle(Response& response) {

	std::string path = getResourcePath(_config.getLocationUri(),
	                                   _config.getRoot(),
	                                   _request.getUri());
// TODO delete
	Utils::printStatus("GET REQUEST HANDLER: resource_path = " + path);

	response.setStatusCode("200");
	if (Utils::isDirectory(path)) {
		handleDirectory(response, path);
	} else if (Utils::isFileExists(path)) {
		handleFile(response, path);
	} else {
	    response.setStatusCode("404");
    }
}

void GetHandler::handleDirectory(Response& response, string& path) {
	string body;
	string pathToIndexFile = findPathToIndexFile(path);

	string extension;

	if (!pathToIndexFile.empty()) {
		handleFile(response, pathToIndexFile);
        extension = Utils::getExtension(pathToIndexFile);
    } else if (_config.isAutoindexEnabled()) {
		extension = "html";
		body = getAutoindexBody(path, _request.findHeaderValue("Host"), _request.getUri());
	} else {
		response.setStatusCode("403");
	}
	if (!body.empty()) {
		setBodyToResponse(response, extension, body);
	}
}

void GetHandler::handleFile(Response& response, string& path) {
	string body = FileReader::readFile(path);

	if (!body.empty()) {
		string extension = Utils::getExtension(path);
		setBodyToResponse(response, extension, body);
	}
}

std::string GetHandler::findPathToIndexFile(string& root) const {
	std::vector<std::string> indexFiles = _config.getIndexFiles();

	if (root.back() != '/') {
		root.append("/");
	}
	for (int i = 0; i < indexFiles.size(); ++i) {
		string indexFileName = indexFiles[i];
		string pathToIndexFile = root + indexFileName;
		if (Utils::isFileExists(pathToIndexFile)) {
			return pathToIndexFile;
		}
	}
	return "";
}

string GetHandler::getAutoindexBody(const std::string& path, const std::string& host, const std::string& uri) {
	std::ostringstream body;
	body << ("<!DOCTYPE html>\n"
	         "<html>\n"
	         "<head>\n"
	         "   <title>  List of files </title>\n"
	         "   <style>h1 {font-size: 200%;\n"
	         "   font-family: Verdana, Arial, Helvetica, sans-serif;\n"
	         "   color: #333366;}\n"
	         "   a {font-size: 100%;"
	         "   font-family: Verdana, Arial, Helvetica, sans-serif;\n"
	         "   color: #333366;}\n"
	         "   li :hover{font-size: 100%;"
	         "   font-family: Verdana, Arial, Helvetica, sans-serif;\n"
	         "   color: #ba2200;}\n"
	         "   li :active{font-size: 100%;"
	         "   font-family: Verdana, Arial, Helvetica, sans-serif;\n"
	         "   color: #918e00;}\n"
	         "<style>footer {\n"
	         "    position: fixed;\n"
	         "    color: #333366;}\n"
	         "    left: 0; bottom: 0;\n"
	         "   color: #333366;}\n"
	         "</style>\n"
	         "</head>\n"
	         "<body>\n"
	         "<h1 align=\"left\"> List of files</h1>");
	std::vector<string> fileNames = getFileNamesFromDirectory(path);
	for (int i = 0; i < fileNames.size(); ++i) {
		body << "<li align=\"left\">"
		     << "<a href =\"http://"
		     << host
		     << uri
		     << "/" + fileNames[i] + "\""
		     << ">" + fileNames[i]
		     << "</a></li><br>";
	}
	body << ("<footer>\n"
	         "&copy;    nbirdper   igearhea     warhchang    2022\n"
	         "</footer>\n"
	         "</body>\n"
	         "</html>\n"
	);
	return body.str();
}

std::vector<std::string> GetHandler::getFileNamesFromDirectory(const std::string& path) {
	std::vector<std::string> fileNames;
	struct dirent* di_struct;
	DIR* dir = opendir(path.c_str());

	if (dir != nullptr) {
		while ((di_struct = readdir(dir)) != nullptr) {
			fileNames.push_back(di_struct->d_name);
		}
		closedir(dir);
	}
	return fileNames;
}


#include "GetHandler.hpp"


BaseHandler* GetHandler::getInstance(Request& request, Config& config) {
	return new GetHandler(request, config);
}

GetHandler::GetHandler(Request& request, Config& config) : BaseHandler(config),
		_request(&request) {
}

GetHandler::~GetHandler() {
}

void GetHandler::handle(Response& response) {
	Utils::printStatus("HANDLE GET");

	std::string path = getResourcePath(_config.getLocationUri(),
	                                   _config.getRoot(),
	                                   _request->getUri());

	Utils::printStatus("GET REQUEST HANDLER: resource_path = " + path);

	response.setStatusCode("200 OK");
	if (Utils::isDirectory(path)) {
		handleDirectory(response, path);
		return;
	}
	if (Utils::isFileExists(path)) {
		handleFile(response, path);
		return;
	}

	Utils::printStatus("RESPONSE 403");
	response.setStatusCode("403 Forbidden");
	string body = getErrorPage("403");
	if (!body.empty()) {
//		string extension = Utils::getExtension(path);
		setBodyToResponse(response, path, body);
	}
}

/*
void GetHandler::setBodyToResponse(Response& response,
                                   const string& extension,
                                   const string& body) {
	response.setBody(body);
	string contentType = _mimeTypesRepo.getTypeByExtension(extension);
	response.addHeader("Content-Type", contentType);
	response.addHeader("Content-Length", std::to_string(body.size()));
}
*/

void GetHandler::handleDirectory(Response& response, string& path) {
	Utils::printStatus("GET REQUEST HANDLER: resource is directory");

	string body;
	string pathToIndexFile = findPathToIndexFile(path);

	string extension;

	if (!pathToIndexFile.empty()) {                             // check index files
		Utils::printStatus("GET REQUEST HANDLER: found index file");
		handleFile(response, pathToIndexFile);
	} else if (_config.isAutoindexEnabled()) {                 // check Autoindex
		Utils::printStatus("GET REQUEST HANDLER: not found index file");
//		extension = _config.getMimeTypeByExtension("html");
		path = ".html";
//		body = getAutoindexBody(path, _request->getUri());
		body = autoindex_alter(path, _request->findHeaderValue("Host"), _request->getUri());
	} else {
		Utils::printStatus("GET REQUEST HANDLER: Index file not found and AUTOINDEX disabled  403 Forbidden");
		response.setStatusCode("403 Forbidden");
		body = getErrorPage("403");
	}
	if (!body.empty()) {
		setBodyToResponse(response, path, body);
	}
}

void GetHandler::handleFile(Response& response, string& path) {
	Utils::printStatus("GET REQUEST HANDLER: resource is a file");

	string body = FileReader::readFile(path);

	if (!body.empty()) {
		string extension = Utils::getExtension(path);
		setBodyToResponse(response, path, body);
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

string GetHandler::getAutoindexBody(const string& path, const string& uri) {

	std::ostringstream body;
	string title = "Index of " + uri;
/*	body.append("<html>\n"
					"<head><title>" + title + "</title></head>\n" +
					"<body>\n" +
					"<h1>" + title + "</h1>"
					"<hr><pre><a href=\"../\">../</a>\n");
*/
	body << "<html>\n"
	     << "<head><title>" << title << "</title></head>\n"
	     << "<body>\n"
	     << "<h1>" << title << "</h1>"
	     << "<hr><pre><a href=\"../\">../</a>\n";

	std::vector<string> fileNames = getFileNamesFromDirectory(path);
	for (int i = 0; i < fileNames.size(); ++i) {
		string fileName = fileNames[i];
		if (strcmp(fileName.c_str(), ".") && strcmp(fileName.c_str(), "..")) {
			string pathWithFileName = path + "/" + fileName;

			struct stat file_stats;
			stat(pathWithFileName.data(), &file_stats);
			bool fileIsDirectory = S_ISDIR(file_stats.st_mode);
			if (fileIsDirectory) {
				fileName.append("/");
			}

			body << "<a href=\"" << uri << fileName << "\">" << fileName << "</a\t";
			/*
			body.append("<a href=\"" + uri + fileName);
			body.append("\">" + fileName);
			body.append("</a>                                               ");
			*/
			time_t lastModified = Utils::getFileModificationDate(pathWithFileName);
			string date = string(ctime(&lastModified));
			date = date.substr(0, date.size() - 1);
//			body.append(date + "                   ");
			body << date << "                   ";
			if (fileIsDirectory) {
//				body.append("-\n");
				body << "-\n";
			} else {
//				body.append(std::to_string(static_cast<float>(file_stats.st_size)) + "\n");
				body << (std::to_string(static_cast<float>(file_stats.st_size)) + "\n");
			}
		}
	}
	body << "</pre><hr></body>\n" <<
	     "</html>\n";
//	body.append("</pre><hr></body>\n"
//				"</html>\n");
//	response->setBody(body);
//	response->setStatusLine("200 OK");
	return body.str();
}

string GetHandler::autoindex_alter(const std::string& path, const std::string& host, const std::string& uri) {
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
//	_responseContentType = getTypeByExtension("html");
//	_responseContentLen = responseBody.str().length();
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


string GetHandler::getErrorPage(const string& errorCode) {
//    string pathToErrorPage = _config.findErrorPagePath("403");
	string pathToErrorPage;
	if (!pathToErrorPage.empty()) {
		return FileReader::readFile(pathToErrorPage);
	}
	return "";
}

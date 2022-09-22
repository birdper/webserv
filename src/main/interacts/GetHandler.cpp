
#include "GetHandler.hpp"

GetHandler::GetHandler() {
}

BaseHandler* GetHandler::getInstance(Request& request, Config& config) {
    return new GetHandler(request, config);
}

GetHandler::GetHandler(Request& request, Config& config) :
        _request(&request),
        _config(&config) {
}

GetHandler::~GetHandler() {

}

void GetHandler::handle(Response& response) {
    Utils::printStatus("HANDLE GET");

    std::string path = getResourcePath(_config->getLocationUri(),
                                       _config->getRoot(),
                                       _request->getUri());

    Utils::printStatus("GET REQUEST HANDLER: after resource_path = " + path);

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
        response.setBody(body);
        response.addHeader("Content-Length", std::to_string(body.size()));
    }
}

void GetHandler::handleDirectory(Response& response, string& path) {
    Utils::printStatus("GET REQUEST HANDLER: resource is directory");

    response.setStatusCode("200 OK");

    string body;
    string pathToIndexFile = findPathToIndexFile(path);

    if (!pathToIndexFile.empty()) {                             // check index files
        Utils::printStatus("GET REQUEST HANDLER: found index file");
        body = FileReader::readFile(pathToIndexFile);
    } else if (_config->isAutoindexEnabled()) {                 // check Autoindex
        Utils::printStatus("GET REQUEST HANDLER: not found index file");
        body = getAutoindexBody(path, _request->getUri());
    } else {
        Utils::printStatus("GET REQUEST HANDLER: AUTOINDEX disabled  403 Forbidden");
        response.setStatusCode("403 Forbidden");
        body = getErrorPage("403");
    }
    if (!body.empty()) {
        response.setBody(body);
//        TODO реализовать mime/types
        response.addHeader("Content-Type", "plain/text");
        response.addHeader("Content-Length", std::to_string(body.size()));
    }
}

std::string GetHandler::findPathToIndexFile(string& root) const {
    std::vector<std::string> indexFiles = _config->getIndexFiles();

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

void GetHandler::handleFile(Response& response, string& path) {
    Utils::printStatus("resource is file");

    response.setStatusCode("200 OK");
    response.setBody(FileReader::readFile(path));
    response.addHeader("Content-Length", std::to_string(response.getBody().size()));
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


#include "GetHandler.hpp"


BaseHandler* GetHandler::getInstance(Request& request, Config& config, MimeTypesRepo& mimeTypeRepo) {
    return new GetHandler(request, config, mimeTypeRepo);
}

GetHandler::GetHandler(Request& request, Config& config, MimeTypesRepo& mimeTypesRepo) :
        _request(&request),
        _config(&config),
        _mimeTypesRepo(mimeTypesRepo) {
}

GetHandler::~GetHandler() {
}

void GetHandler::handle(Response& response) {
    Utils::printStatus("HANDLE GET");

    std::string path = getResourcePath(_config->getLocationUri(),
                                       _config->getRoot(),
                                       _request->getUri());

    Utils::printStatus("GET REQUEST HANDLER: after resource_path = " + path);

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
        string extension = Utils::getExtension(path);
        setBodyToResponse(response, extension, body);
    }
}

void GetHandler::setBodyToResponse(Response& response,
                                   const string& extension,
                                   const string& body) {
    response.setBody(body);
    string contentType = _mimeTypesRepo.getTypeByExtension(extension);
    response.addHeader("Content-Type", contentType);
    response.addHeader("Content-Length", std::to_string(body.size()));
}

void GetHandler::handleDirectory(Response& response, string& path) {
    Utils::printStatus("GET REQUEST HANDLER: resource is directory");

    string body;
    string pathToIndexFile = findPathToIndexFile(path);

    string extension;

    if (!pathToIndexFile.empty()) {                             // check index files
        Utils::printStatus("GET REQUEST HANDLER: found index file");
        handleFile(response, pathToIndexFile);
    } else if (_config->isAutoindexEnabled()) {                 // check Autoindex
        Utils::printStatus("GET REQUEST HANDLER: not found index file");
        extension = _mimeTypesRepo.getTypeByExtension("html");
        body = getAutoindexBody(path, _request->getUri());
    } else {
        Utils::printStatus("GET REQUEST HANDLER: AUTOINDEX disabled  403 Forbidden");
        response.setStatusCode("403 Forbidden");
        body = getErrorPage("403");
    }
    if (!body.empty()) {
        setBodyToResponse(response, extension, body);
    }
}

void GetHandler::handleFile(Response& response, string& path) {
    Utils::printStatus("GET REQUEST HANDLER: resource is a file");

    string body = FileReader::readFile(path);

    if (!body.empty()) {
        string extension = Utils::getExtension(path);
        setBodyToResponse(response, extension, body);
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

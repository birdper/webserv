
#include "GetHandler.hpp"

//GetHandler::GetHandler(Request& request, Config& config) :
//        BaseHandler(request, config)
//        _request(request),
//        _config(config) {
//}

GetHandler::GetHandler(Request& request, Config& config) :
        BaseHandler() {
}

GetHandler::~GetHandler() {

}

Response GetHandler::handle(Request& request, Config& config) {
    Response response;
    const string path = getPathFromUri();

//    isDirectory
    if (Utils::isDirectory(path)) {

//        check index files
        _config.getIndexFiles();
//        if (isNotFoundIndexFile)

//        check autoindex
        if (_config.isAutoindexEnabled()) {
            string body = getAutoindexBody(getFileNamesFromDirectory(path),
                                           path,
                                           _request.getUri());
            response.addHeader("Content-Length", std::to_string(body.size()));
            response.setStatusCode("200 OK");
        } else {
            response.setStatusCode("403 Forbidden");
        }
    } else {
        readfileToBody(response, path);
    }
    return response;
}

string GetHandler::getAutoindexBody(std::vector<string> fileNames,
                                             const string& path,
                                             const string& uri) {
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

std::string GetHandler::getPathFromUri() const {

    std::string path = _config.getRoot() + "/" + _request.getUri();

    if (Utils::isFileExists(path)) {
        if (Utils::isDirectory(path)) {
            if (path.back() != '/') {
                path.append("/");
            }

            std::vector<std::string> indexes = _config.getIndexFiles();
            for (int i = 0; i < indexes.size(); ++i) {
                std::string indexFileName = indexes[i];
                std::string pathToIndexFile;
                if (indexFileName.front() == '/') {
                    pathToIndexFile = _config.getRoot() + indexFileName;
                } else {
                    pathToIndexFile = path + indexFileName;
                }
                if (Utils::isFileExists(pathToIndexFile) && !Utils::isDirectory(pathToIndexFile) &&
                    !access(pathToIndexFile.c_str(), W_OK)) {
                    return pathToIndexFile;
                }
            }
        }
        return path;
    }
    return std::string();
}

BaseHandler* GetHandler::getInstance() {
    return new GetHandler();
}

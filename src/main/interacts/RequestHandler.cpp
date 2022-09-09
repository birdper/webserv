#include "RequestHandler.hpp"

/*
RequestHandler::RequestHandler(Config& config) :
		config(config) {}
*/

Response RequestHandler::handle(Request& request, Config& config) {

	Response response;
//	if (request.isBadRequest())
//		return response;
	validate(request, config);

	switch (request.getMethod()) {
		case GET:
			handleGet(request, response, config);
			break;
		case PUT:
//			handleGet(client, request);
			break;
		case POST:
//			handleGet(client, request);
			break;
		case DELETE:
//			handleGet(client, request);
			break;
		default:
//			sendStatusResponse(client, Status(NOT_IMPLEMENTED));
			break;
	}
	return response;
}

bool RequestHandler::validate(Request& request, Config& config) {
//	request.getMethod()
	return true;
}

void RequestHandler::handleGet(Request& request, Response& response, Config& config) {
	const std::string path = getPathFromUri(request.getUri(), config);

	if (Utils::isDirectory(path)) {
		if (config.isAutoindexEnabled()) {
			std::string body = getAutoindexBody(getFileNamesFromDirectory(path),
												path,
												request.getUri());
			response.setStatusCode("200 OK");
		}
		else {
			response.setStatusCode("403 Forbidden");
		}
	}
	else {
		readfile(response, path);
	}
}

std::string RequestHandler::getAutoindexBody(std::vector<std::string> fileNames,
											 const std::string& path,
											 const std::string& uri) {
	std::ostringstream body;

	std::string title = "Index of " + uri;
/*	body.append("<html>\n"
					"<head><title>" + title + "</title></head>\n" +
					"<body>\n" +
					"<h1>" + title + "</h1>"
					"<hr><pre><a href=\"../\">../</a>\n")*/;
	body << "<html>\n"
			<< "<head><title>" << title << "</title></head>\n"
			<< "<body>\n"
			<< "<h1>" << title << "</h1>"
			<< "<hr><pre><a href=\"../\">../</a>\n";
	for (int i = 0; i < fileNames.size(); ++i) {
		std::string fileName = fileNames[i];
		if (strcmp(fileName.c_str(), ".") && strcmp(fileName.c_str(), "..")) {
			std::string pathWithFileName = path + "/" + fileName;

			struct stat file_stats;
			stat(pathWithFileName.data(), &file_stats);
			bool fileIsDirectory = S_ISDIR(file_stats.st_mode);
			if (fileIsDirectory) {
				fileName.append("/");
			}

			body << "<a href=\"" << uri << fileName << "\">" << fileName << "</a\t";
//			body.append("<a href=\"" + uri + fileName);
//			body.append("\">" + fileName);
//			body.append("</a>                                               ");
			time_t lastModified = Utils::getFileModificationDate(pathWithFileName);
			std::string date = std::string(ctime(&lastModified));
			date = date.substr(0, date.size() - 1);
//			body.append(date + "                   ");
			body << date << "                   ";
			if (fileIsDirectory) {
//				body.append("-\n");
				body << "-\n";
			}
			else {
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

std::vector<std::string> RequestHandler::getFileNamesFromDirectory(const std::string& path) {
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

void RequestHandler::handlePost(Request& request) {

}

//TODO проверить
std::string getRedirectPageBody(std::pair<int, std::string> redirect) {
	int redirectCode = redirect.first;
	if ((redirectCode > 300 && redirectCode < 303) || redirectCode == 307 || redirectCode == 308) {
		std::string redirectMsg = std::to_string(redirectCode);
		return "<html>\n"
			   "<head><title>" + redirectMsg + "</title></head>\n" + "<body>\n" + "<center><h1>" +
			   redirectMsg + "</h1></center>\n" + "<hr><center>webserv</center>\n" + "</body>\n" +
			   "</html>";
	}
	else
		return redirect.second;
}

std::string RequestHandler::getPathFromUri(const std::string& uri, Config& config) const {

	std::string path = config.getRoot() + "/" + uri;

	if (Utils::isFileExists(path)) {
		if (Utils::isDirectory(path)) {
			if (path.back() != '/') {
				path.append("/");
			}

			std::vector<std::string> indexes = config.getIndexFiles();
			for (int i = 0; i < indexes.size(); ++i) {
				std::string indexFileName = indexes[i];
				std::string pathToIndexFile;
				if (indexFileName.front() == '/') {
					pathToIndexFile = config.getRoot() + indexFileName;
				}
				else {
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

// TODO Доделать вычитываение файла в body
void RequestHandler::readfile(Response& response, const std::string& path) {
	std::string extension = path.substr(path.find_last_of("/\\") + 1);
	std::string fileExtension = path.substr(path.find_last_of('.') + 1);

//	response.setContentType(mimeType(extension));
	try {
		response.setBody(FileReader::readFile(path));
		response.setStatusCode("200");
	} catch (FileNotFoundException& ex) {
//		response.setBody(getErrorPageBody("ДОБРО ПОЖАЛОВАТЬ НА СТРАНИЦУ 404!"));
		response.setStatusCode("404");
	}
}

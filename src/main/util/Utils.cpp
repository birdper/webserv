//
// Created by Aleksei S on 07.09.2022.
//

#include "Utils.hpp"

std::string Utils::getCurrentTimestamp(bool localtime, const std::string& format) {
	time_t rawtime = std::time(nullptr);

	struct tm* tm_info = nullptr;
	if (localtime) {
		tm_info = std::localtime(&rawtime);
	}
	else {
		tm_info = std::gmtime(&rawtime);
	}

	char buffer[30] = {'\0'};
	strftime(buffer, sizeof(buffer), format.c_str(), tm_info);

	return std::string(buffer);
}

bool Utils::isFileExists(const std::string& pathToFile) {
//	std::ifstream file(pathToFile);
//	return file.is_open();
	struct stat file;
	return (stat(pathToFile.c_str(), &file) == 0);
}

bool Utils::isDirectory(const std::string& pathToFile) {
	struct stat file;
	return stat(pathToFile.c_str(), &file) != -1 && S_ISDIR(file.st_mode);
}

bool Utils::isAccessRights(const std::string& pathToFile) {
	struct stat file;
	return stat(pathToFile.c_str(), &file) != -1 && file.st_mode & S_IROTH;
}

time_t Utils::getFileModificationDate(const std::string& pathToFile) {
	struct stat file;
	return (stat(pathToFile.c_str(), &file) != -1) ? file.st_mtime : -1;
}

//
// Created by Aleksei S on 07.09.2022.
//

#include "Utils.hpp"

string Utils::getCurrentTimestamp(bool localtime, const string& format) {
    time_t rawtime = std::time(nullptr);

    struct tm* tm_info = nullptr;
    if (localtime) {
        tm_info = std::localtime(&rawtime);
    } else {
        tm_info = std::gmtime(&rawtime);
    }

    char buffer[30] = {'\0'};
    strftime(buffer, sizeof(buffer), format.c_str(), tm_info);

    return string(buffer);
}

void Utils::printStatus(const string& message) {
    cout << Utils::getCurrentTimestamp(true) << ": " << message << endl;
}

bool Utils::isFileExists(const string& pathToFile) {
//    TODO delete unuse comments
//	std::ifstream file(pathToFile);
//	return file.is_open();
    struct stat file;
    return (stat(pathToFile.c_str(), &file) == 0);
}

bool Utils::isDirectory(const string& pathToFile) {
    struct stat file;
    return stat(pathToFile.c_str(), &file) != -1 && S_ISDIR(file.st_mode);
}

bool Utils::isAccessRights(const string& pathToFile) {
    struct stat file;
    return stat(pathToFile.c_str(), &file) != -1 && file.st_mode & S_IROTH;
}

time_t Utils::getFileModificationDate(const string& pathToFile) {
    struct stat file;
    return (stat(pathToFile.c_str(), &file) != -1) ? file.st_mtime : -1;
}

/** for parsers */
std::vector<string> Utils::split(const string& str, const string& delimiter) {
    std::vector<string> tokens;
    size_t previousPosition = 0;
    size_t currentPosition = 0;
    do {
        currentPosition = str.find(delimiter, previousPosition);
        if (currentPosition == string::npos) {
            currentPosition = str.length();
        }
        string token = str.substr(previousPosition, currentPosition - previousPosition);
        if (!token.empty()) {
            tokens.push_back(token);
        }
        previousPosition = currentPosition + delimiter.length();
    } while (currentPosition < str.length() && previousPosition < str.length());

    return tokens;
}

string Utils::ft_toLower(const string& line) {
    string lowerString;

    for (int i = 0; i < line.length(); ++i) {
        lowerString += (char) std::tolower(line[i]);
    }
    return lowerString;
}

string Utils::ltrim(string str, const string& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

string Utils::rtrim(string str, const string& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string Utils::getExtension(const string& fileName) {
    std::string ext;
    size_t ext_pos = fileName.find_last_of(".");
    if (ext_pos != std::string::npos)
        ext = fileName.substr(ext_pos + 1);
    return ext;
}

string Utils::getFileName(const string& path) {
    size_t pos = path.find_last_of('/');
    return (pos != string::npos) ? path.substr(pos + 1) : "";
}

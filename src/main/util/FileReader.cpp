//
// Created by Aleksei S on 08.09.2022.
//

#include "FileReader.hpp"
#include "FileNotFoundException.hpp"

std::string FileReader::readFile(const std::string& fileName) {
    std::ifstream inFile(fileName, std::ios::binary);
    std::string result;

    inFile.seekg(0, std::ios::end);
    try {
        result.resize(inFile.tellg());
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    inFile.seekg(0, std::ios::beg);
    inFile.read((char*)result.data(), result.size());
    inFile.close();
    return result;
}

std::string FileReader::readFile2(const std::string& fileName) {
	std::ifstream ifstream(fileName, std::ios::binary);
	std::string result;

	try {
 	ifstream.seekg(0, std::ios::end);
    result.reserve(ifstream.tellg());
    ifstream.seekg(0, std::ios::beg);
    result.assign((std::istreambuf_iterator<char>(ifstream)),
                  std::istreambuf_iterator<char>());
	} catch (std::exception& e) {
		std::cerr << "readFile(): " << e.what() << std::endl;
	}
    ifstream.close();
	return result;
}

std::string
FileReader::file_contents(const std::string& path)
{
    // Sanity check
//    if (!std::filesystem::is_regular_file(path))
//        return { };

    // Open the file
    // Note that we have to use binary mode as we want to return a string
    // representing matching the bytes of the file on the file system.
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw FileNotFoundException(path);

    // Read contents
    std::string content;
    content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Close the file
    file.close();

    return content;
}
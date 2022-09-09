//
// Created by Aleksei S on 08.09.2022.
//

#include "FileReader.hpp"

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

#include <string>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <iostream>

#pragma once
class getMapOpen
{
public:
	std::string getMapOpen1(const std::string& folder_path);
private:
	void checkFile(const std::filesystem::path& filePath, std::string& nameFile);

	bool containsOnlyEnglishCharacters(const std::string& text);

	std::mutex m_Mtx;
	std::string m_nameFile;
};


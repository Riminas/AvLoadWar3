#include <iostream>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <unordered_map>
#include <Windows.h>
#include <ShlObj_core.h>
#include "StringToString.h"
#include "DataPath.h"
#include <unordered_set>

// Utility function to convert wide string to string
std::string wideStringToString(const std::wstring& wStr) {
    return std::string(wStr.begin(), wStr.end());
}

// Utility function to remove specific characters from a string
void removeSpecialCharacters(std::string& str) {
    for (char& c : str) {
        if (c == '(' || c == ')' || c == ':' || c == '"') {
            c = ' ';
        }
    }
}

// Utility function to convert string to lowercase
std::string toLower(const std::string& str) {
    std::string result;
    for (const char& c : str) {
        result += std::tolower(c);
    }
    return result;
}

bool DataPath::initializePaths() {
    PWSTR path = NULL;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);
    if (!SUCCEEDED(hr)) {
        std::cout << "Error: SHGetKnownFolderPath()" << std::endl;
        return false;
    }

    std::string str = wideStringToString(path) + "\\Warcraft III";
    CoTaskMemFree(path);

    if (!std::filesystem::is_directory(str)) {
        std::cout << "Error: directory Warcraft III(" << str << ")" << std::endl;
        return false;
    }

    m_Maps = str + "\\maps";
    std::cout << "Warcraft Maps Path: " << m_Maps << std::endl;
    m_Save = str + "\\CustomMapData";
    std::cout << "Warcraft Save Path: " << m_Save << std::endl;

    return true;
}

std::vector<DataPath::HeroInfo> DataPath::retrieveHeroData(const std::string& saveCodePath, bool& isError) {
    std::string fullSavePath = m_Save + saveCodePath;

    if (!std::filesystem::is_directory(fullSavePath)) {
        std::cout << "Error: directory - (" << fullSavePath << ")" << std::endl;
        isError = true;
        return m_HeroList;
    }

    m_HeroList = parseHeroData(fullSavePath);
    return m_HeroList;
}

std::vector<DataPath::HeroInfo> DataPath::parseHeroData(const std::string& folderPath) {
    std::unordered_map<std::string, HeroInfo> heroDataMap;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_directory()) {
            processDirectory(entry, heroDataMap);
        }
        else if (isTextFile(entry)) {
            processFile(entry, heroDataMap);
        }
    }

    return convertToHeroList(heroDataMap);
}

void DataPath::processDirectory(const std::filesystem::directory_entry& dirEntry, std::unordered_map<std::string, HeroInfo>& heroDataMap) {
    HeroInfo& heroInfo = heroDataMap[dirEntry.path().filename().string()];

    for (const auto& entry : std::filesystem::directory_iterator(dirEntry.path())) {
        if (isTextFile(entry)) {
            updateHeroInfo(entry, heroInfo);
        }
    }
}

void DataPath::processFile(const std::filesystem::directory_entry& fileEntry, std::unordered_map<std::string, HeroInfo>& heroDataMap) {
    std::filesystem::file_time_type fileTime = fileEntry.last_write_time();
    std::ifstream file(fileEntry.path());

    std::string line;
    while (file && std::getline(file, line)) {
        removeSpecialCharacters(line);
        std::istringstream iss(line);
        std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

        for (size_t i = 0; i < tokens.size(); ++i) {
            if (isHeroToken(toLower(tokens[i]))) {
                std::string heroName = extractHeroName(tokens, i);
                HeroInfo& heroInfo = heroDataMap[heroName];
                if (fileTime > heroInfo.latestTime) {
                    heroInfo.latestTime = fileTime;
                    heroInfo.path = fileEntry.path().string();
                }
                return;
            }
        }
    }
}

bool DataPath::isTextFile(const std::filesystem::directory_entry& entry) const {
    return std::filesystem::is_regular_file(entry) && entry.path().extension() == ".txt";
}

bool DataPath::isHeroToken(const std::string& token) const {
    static const std::unordered_set<std::string> heroTokens = { "hero", "job", "char", "character", "перс" };
    return heroTokens.count(token) > 0;
}

std::string DataPath::extractHeroName(const std::vector<std::string>& tokens, size_t startIndex) const {
    std::string heroName = tokens[startIndex];
    for (size_t i = startIndex + 1; i < tokens.size(); ++i) {
        heroName += " " + tokens[i];
    }
    return heroName;
}

void DataPath::updateHeroInfo(const std::filesystem::directory_entry& fileEntry, HeroInfo& heroInfo) {
    std::filesystem::file_time_type fileTime = fileEntry.last_write_time();
    if (fileTime > heroInfo.latestTime) {
        heroInfo.latestTime = fileTime;
        heroInfo.path = fileEntry.path().string();
    }
}

std::vector<DataPath::HeroInfo> DataPath::convertToHeroList(const std::unordered_map<std::string, HeroInfo>& heroDataMap) const {
    std::vector<HeroInfo> heroList;
    for (const auto& [name, info] : heroDataMap) {
        heroList.push_back(HeroInfo(name, info.path, info.latestTime));
        if (heroList.size() > 9) {
            break;
        }
    }
    return heroList;
}

std::string DataPath::getMapsPath() const { return m_Maps; }
std::string DataPath::getSavePath() const { return m_Save; }
std::vector<DataPath::HeroInfo> DataPath::getHeroList() const { return m_HeroList; }

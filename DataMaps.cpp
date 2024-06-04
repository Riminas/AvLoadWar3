#include <vector>
#include <ranges>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm>

#include "StringToString.h"
#include "NewPathSaveCode.h"
#include "DataMaps.h"

namespace fs = std::filesystem;

void DataMaps::NameMaps(const std::string& fileName) {
    StringToString stringToString;
    auto nameFileRemove = stringToString.removeString(fileName);

    m_NameMaps = nameFileRemove[0];
    for (const auto& part : nameFileRemove | std::views::drop(1)) {
        m_NameMaps += '_' + part;
    }
    m_NameMaps += ".txt";
}

bool DataMaps::PutSaveCode(const std::wstring& path) {
    if (loadDatFail()) {
        std::cout << "Loads: " << m_PutSaveCode << std::endl;
        return true;
    }

    if (PathDirectorySaveCode(path)) {
        std::cout << "Loads: " << m_PutSaveCode << std::endl;
        return true;
    }

    std::cout << "Error: No information file (" << m_putDatMapPut << "\\" << m_NameMaps << ")\n";
    std::cout << "Create file (" << m_NameMaps << ") and save code directory\n";

    NewPathSaveCode newPathSaveCode(path, m_NameMaps);
    if (!newPathSaveCode.newPathSaveCode()) {
        return false;
    }

    if (loadDatFail()) {
        std::cout << "Loads: " << m_PutSaveCode << std::endl;
        return true;
    }

    std::cout << "Error: No information file (" << m_putDatMapPut << "\\" << m_NameMaps << ")\n";
    return false;
}

bool DataMaps::loadDatFail() {
    std::ifstream inFile(m_putDatMapPut + '\\' + m_NameMaps);
    if (!inFile.is_open()) {
        return false;
    }

    std::getline(inFile, m_PutSaveCode);
    return true;
}

std::string DataMaps::removePath(const std::string& filePath) {
    size_t start_pos = filePath.find("CustomMapData");
    if (start_pos == std::string::npos) {
        std::cerr << "Substring 'CustomMapData' not found in the path." << std::endl;
        return "";
    }
    start_pos += std::string("CustomMapData").length() + 1;

    size_t end_pos = filePath.find_last_of("\\");
    if (end_pos == std::string::npos) {
        std::cerr << "Last backslash not found in the path." << std::endl;
        return "";
    }

    return "\\" + filePath.substr(start_pos, end_pos - start_pos);
}

bool DataMaps::PathDirectorySaveCode(const std::wstring& path) {
    std::string substr = m_NameMaps;
    if (substr.size() > 4 && substr.substr(substr.size() - 4) == ".txt") {
        substr.erase(substr.size() - 4);
    }

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (fs::is_directory(entry.path())) {
            std::string result("\0");
            std::string str = entry.path().filename().string();
            std::transform(str.begin(), str.end(), std::back_inserter(result), [](char c) {
                return std::tolower(c);
                });


            if (result == substr) {
                m_PutSaveCode = removePath(entry.path().string());
                return !m_PutSaveCode.empty();
            }
        }
    }
    return false;
}

std::string DataMaps::getPutSaveCode() const { return m_PutSaveCode; }
std::string DataMaps::getNameMaps() const { return m_NameMaps; }
std::string DataMaps::getPutDatMapPut() const { return m_putDatMapPut; }

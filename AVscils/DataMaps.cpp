#include <vector>
#include <ranges>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <algorithm>

#include "StringToString.h"
#include "DataMaps.h"
#include <ShlObj.h>

namespace fs = std::filesystem;

void DataMaps::NameMaps(const std::wstring& fileName) {
    StringToString stringToString;
    auto nameFileRemove = stringToString.removeString(fileName);

    m_NameMaps = nameFileRemove[0];
    for (const std::wstring& part : nameFileRemove | std::views::drop(1)) {
        m_NameMaps += L'_' + part;
    }
}

int DataMaps::PutSaveCode(const std::wstring& path) {
    if(m_PutSaveCode == L"False")
        return 2;

    if (loadDatFail(path)) {
        std::wcout << "Loads1: " << m_PutSaveCode << std::endl;
        return 1;
    }

    if (PathDirectorySaveCode(path)) {
        std::wcout << "Loads2: " << m_PutSaveCode << std::endl;
        return 1;
    }

    m_PutSaveCode = L"False";
    std::wcout << L"Error: No information file (" << m_pathDatMap << L"\\" << m_NameMaps << m_pathFile << L")\n";
    return 0;
}

std::wstring DataMaps::utf8_to_utf16(const std::string& str) {
    if (str.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

bool DataMaps::loadDatFail(const std::wstring& fullPath) {
    std::wstring str = m_pathDatMap + L'\\' + m_NameMaps + m_pathFile;
    std::ifstream inFile(str, std::ios::binary);
    if (!inFile.is_open()) {
        return false;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    std::string utf8Text(buffer.begin(), buffer.end());

    // Конвертируем прочитанный текст в std::wstring
    m_PutSaveCode = utf8_to_utf16(utf8Text);

    if(std::filesystem::is_directory((fullPath + m_PutSaveCode)))
        return true;
    return false;
}

std::wstring DataMaps::removePath(const std::wstring& filePath) {
    size_t start_pos = filePath.find(L"CustomMapData");
    if (start_pos == std::wstring::npos) {
        std::cerr << "Substring 'CustomMapData' not found in the path." << std::endl;
        return L"";
    }
    start_pos += std::wstring(L"CustomMapData").length() + 1;

    size_t end_pos = filePath.find_last_of(L"\\");
    if (end_pos == std::wstring::npos) {
        std::cerr << "Last backslash not found in the path." << std::endl;
        return L"";
    }

    return L"\\" + filePath.substr(start_pos, end_pos - start_pos);
}

bool DataMaps::PathDirectorySaveCode(const std::wstring& path) {
    std::wstring substr = m_NameMaps;
    if (substr.size() > 4 && substr.substr(substr.size() - 4) == L".txt") {
        substr.erase(substr.size() - 4);
    }

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (fs::is_directory(entry.path())) {
            std::wstring result(L"\0");
            std::wstring str = entry.path().filename().wstring();
            std::transform(str.begin(), str.end(), std::back_inserter(result), [](char c) {
                return std::tolower(c);
                });


            if (result == substr) {
                m_PutSaveCode = removePath(entry.path().wstring());
                return !m_PutSaveCode.empty();
            }
        }
    }
    return false;
}

std::wstring DataMaps::getPutSaveCode() const { return m_PutSaveCode; }
std::wstring DataMaps::getNameMaps() const { return m_NameMaps; }
std::wstring DataMaps::getPutDatMapPut() const { return m_pathDatMap; }

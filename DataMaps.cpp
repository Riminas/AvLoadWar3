#include <vector>
#include <ranges>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <shlobj.h>

#include "StringToString.h"
#include "DataMaps.h"

namespace fs = std::filesystem;

bool DataMaps::loadDataMaps(const std::string& fileName, const std::wstring& path) {
    NameMaps(fileName);
    return PutSaveCode(path);
}

void DataMaps::NameMaps(const std::string& fileName) {
    StringToString StringToString_;
    auto nameFileRemove = StringToString_.removeString(fileName);

    m_NameMaps = nameFileRemove[0];
    for (const auto& p : nameFileRemove | std::views::drop(1))
        m_NameMaps += '_' + p;
    m_NameMaps += ".txt";
}

bool DataMaps::PutSaveCode(const std::wstring& path) {
    if (loadDatFail()) {
        std::cout << "Loads: " << m_PutSaveCode << std::endl;
        return true;
    }

    std::cout << "Error: Not information file (" << m_putDatMapPut << "\\" << m_NameMaps << ")\n";
    std::cout << "Create file (" << m_NameMaps << ") and save code directory\n";

    auto folderPath = BrowseFolderDialog(path);
    std::string filePath(folderPath.begin(), folderPath.end());
    if (filePath.empty() || (filePath = removePath(filePath)).empty())
        return false;

    std::ofstream outFile(m_putDatMapPut + '\\' + m_NameMaps);
    if (outFile) {
        outFile << filePath;
        outFile.close();
    }
    else {
        std::cerr << "Failed to create file: " << m_putDatMapPut << '\\' << m_NameMaps << '\n';
        return false;
    }

    if (loadDatFail()) {
        std::cout << "Loads: " << m_PutSaveCode << std::endl;
        return true;
    }

    std::cout << "Error: Not information file (" << m_putDatMapPut << "\\" << m_NameMaps << ")\n";
    return false;
}

bool DataMaps::loadDatFail() {
    std::ifstream inFile(m_putDatMapPut + '\\' + m_NameMaps);
    if (!inFile.is_open())
        return false;

    std::getline(inFile, m_PutSaveCode);
    return true;
}

LPITEMIDLIST GetPIDLFromPath(const std::wstring& path) {
    LPITEMIDLIST pidl = nullptr;
    HRESULT hr = SHParseDisplayName(path.c_str(), nullptr, &pidl, 0, nullptr);
    return SUCCEEDED(hr) ? pidl : nullptr;
}

std::wstring DataMaps::BrowseFolderDialog(const std::wstring& initialPath) {
    LPITEMIDLIST pidlRoot = GetPIDLFromPath(initialPath);

    BROWSEINFO bi = { 0 };
    bi.lpszTitle = L"Specify the directory where the save files are stored";
    bi.pidlRoot = pidlRoot;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    std::wstring result;

    if (pidl) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDList(pidl, path)) {
            result = path;
        }

        IMalloc* imalloc = nullptr;
        if (SUCCEEDED(SHGetMalloc(&imalloc))) {
            imalloc->Free(pidl);
            imalloc->Release();
        }
    }
    return result;
}

std::string DataMaps::removePath(const std::string& filePath) {
    std::string path = filePath;
    const std::string marker = "CustomMapData";
    size_t start_pos = path.find(marker);
    if (start_pos == std::string::npos) {
        std::cerr << "Substring 'CustomMapData' not found in the path.\n";
        return "";
    }
    start_pos += marker.length() + 1;

    size_t end_pos = path.find_last_of("\\");
    if (end_pos == std::string::npos) {
        std::cerr << "Last backslash not found in the path.\n";
        return "";
    }

    return "\\" + path.substr(start_pos, end_pos - start_pos);
}

std::string DataMaps::getPutSaveCode() const { return m_PutSaveCode; }
std::string DataMaps::getNameMaps() const { return m_NameMaps; }
std::string DataMaps::getPutDatMapPut() const { return m_putDatMapPut; }

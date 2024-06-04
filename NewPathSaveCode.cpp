#include <iostream>
#include <fstream>
#include <ShlObj.h>

#include "NewPathSaveCode.h"

bool NewPathSaveCode::newPathSaveCode() {
    auto folderPath = BrowseFolderDialog();
    std::string filePath(folderPath.begin(), folderPath.end());
    if (filePath.empty() || (filePath = extractSubPath(filePath)).empty())
        return false;

    std::ofstream outFile(m_putDatMapPut + '\\' + m_NameMaps);
    if (outFile) {
        outFile << filePath;
        outFile.close();
        std::cout << "New path SaveCode ( " << m_putDatMapPut + '\\' + m_NameMaps << " )" << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to create file: " << m_putDatMapPut << '\\' << m_NameMaps << '\n';
        return false;
    }
    return true;
}

LPITEMIDLIST NewPathSaveCode::GetPIDLFromPath() {
    LPITEMIDLIST pidl = nullptr;
    HRESULT hr = SHParseDisplayName(m_Path.c_str(), nullptr, &pidl, 0, nullptr);
    return SUCCEEDED(hr) ? pidl : nullptr;
}

std::wstring NewPathSaveCode::BrowseFolderDialog() {
    LPITEMIDLIST pidlRoot = GetPIDLFromPath();

    BROWSEINFO bi = { 0 };
    std::wstring wstr{ m_NameMaps.begin(), m_NameMaps.end() };
    wstr = L"Specify the directory where the save files are stored ( " + wstr + L" )";
    bi.lpszTitle = wstr.c_str();
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

std::string NewPathSaveCode::extractSubPath(const std::string& t_Path) {
    std::string path = { t_Path.begin(), t_Path.end() };
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
    if (start_pos >= path.size()) {
        std::cerr << "start_pos >= path_size.\n";
        return "";
    }
    std::string str = path.substr(start_pos, end_pos - start_pos);
    return "\\" + str;
}
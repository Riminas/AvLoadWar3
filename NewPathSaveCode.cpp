#include <iostream>
#include <ShlObj.h>
#include <filesystem>
#include <windows.h>
#include <shobjidl.h>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <fstream>
#include <cstdio>

#include "StringConvector.h"
#include "LoadDataFail.h"
#include "NewPathSaveCode.h"

void NewPathSaveCode::saveFile(const std::wstring& t_Path, std::ofstream& outFile) {
    StringConvector StringConvector_;
    std::string utf8Text = StringConvector_.utf16_to_utf8(t_Path);
    outFile.write(utf8Text.c_str(), utf8Text.size());
    outFile.close();
}

bool NewPathSaveCode::newPathSaveCode(std::wstring& t_Path) {
    {
        LoadDataFail LoadDataFail_;
        t_Path = LoadDataFail_.loadDataFail(m_PathOptionDirectory);

        std::filesystem::path path{ t_Path };
        std::wstring wstr = path.parent_path().wstring();

        if (std::filesystem::is_directory(wstr)) {
            return true;
        }
    } 
    {
        std::ofstream outFile(m_PathOptionDirectory, std::ios::binary);
        t_Path = BrowseFileDialog();
        saveFile(t_Path, outFile);
    }
    return true;
}

std::wstring NewPathSaveCode::BrowseFileDialog() const {
    OPENFILENAME ofn;
    wchar_t szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
    ofn.lpstrFilter = L"Warcraft III Executable (war3.exe)\0war3.exe\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn) == TRUE) {
        std::wstring filePath(ofn.lpstrFile);
        return filePath;
    }

    return L"";
}

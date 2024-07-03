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

#include "NewPathSaveCode.h"
#include "StringConvector.h"

void NewPathSaveCode::loadFile(std::wstring& t_Path) {
    // Проверка существования файла
    if (!std::filesystem::exists(m_PathOptionDirectory)) {
        return;
    }

    // Получение размера файла
    auto fileSize = std::filesystem::file_size(m_PathOptionDirectory);

    // Открытие файла
    FILE* file = nullptr;
    std::string str2 = { m_PathOptionDirectory.begin(),m_PathOptionDirectory.end() };
    errno_t err = fopen_s(&file, str2.c_str(), "rb");
    if (err == 0 && file != nullptr) {
        // Чтение файла в буфер
        std::vector<char> buffer(fileSize);
        fread(buffer.data(), 1, fileSize, file);
        fclose(file);

        // Преобразование буфера в строку
        std::string str(buffer.begin(), buffer.end());
        StringConvector StringConvector_;
        t_Path = StringConvector_.utf8_to_utf16(str);
    }
    else {
        return;
    }
}

void NewPathSaveCode::saveFile(const std::wstring& t_Path, std::ofstream& outFile) {
    StringConvector StringConvector_;
    std::string utf8Text = StringConvector_.utf16_to_utf8(t_Path);
    outFile.write(utf8Text.c_str(), utf8Text.size());
    outFile.close();
}

bool NewPathSaveCode::newPathSaveCode(std::wstring& t_Path) {
    {
        loadFile(t_Path);
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

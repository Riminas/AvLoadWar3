#pragma once
#include "DataAll.h"
#include <string>

class LoadDataCommands {
public:
    LoadDataCommands(DataAll& m_DataAll);

    void loadDataCommands();
    void saveDataCommands();
private:
    void writeDefaultDataCommands(const std::wstring& fullPath, const bool isDataDefaul);
    void load();
    void parseCommandLine(const std::wstring& line);
    bool starDataCommands();

    bool m_IsShouldRewrite = false;

    std::wstring m_FileName = L"\0";
    std::wstring m_FullPath = L"\0";
    std::string m_LastCommand; // Added to track the last command

    const std::wstring m_putDatMapPut = L"DataMaps";
    const std::wstring m_DataCommandsPath = L"\\DataCommands.dat";
    const std::wstring m_ComandsPath = L"\\Comands.dat";

    DataAll& m_DataAll;
};
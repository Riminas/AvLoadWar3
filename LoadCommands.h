#pragma once
#include "DataAll.h"
#include <string>


class LoadCommands {
public:
    LoadCommands(DataAll& m_DataAll);

    void loadCommands();
    void saveCommands();
private:
    void writeDefaultCommands(const std::wstring& fullPath);
    void load();
    void parseCommandLine(const std::string& line);
    bool starCommands();

    std::wstring m_FileName = L"\0";
    std::wstring m_FullPath = L"\0";

    const std::wstring m_putDatMapPut = L"DataMaps";
    const std::wstring m_CommandsPath = L"\\Commands.dat";
    const std::wstring m_ComandsPath = L"\\Comands.dat";

    DataAll& m_DataAll;
};
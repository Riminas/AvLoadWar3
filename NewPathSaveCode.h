#pragma once
#include <string>
#include <ShObjIdl.h>
class NewPathSaveCode {
public:
    NewPathSaveCode(const std::wstring& path, const std::string& nameMaps)
        : m_Path(path), m_NameMaps(nameMaps) {}

    bool newPathSaveCode();

private:
    std::wstring BrowseFolderDialog();
    LPITEMIDLIST GetPIDLFromPath();
    std::string extractSubPath(const std::string& t_Path);

    std::wstring m_Path;
    std::string m_NameMaps;
    std::string m_putDatMapPut = "dataAvLoad\\DataMaps"; // Замените на ваш путь
};
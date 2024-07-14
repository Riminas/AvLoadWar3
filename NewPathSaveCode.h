#pragma once
#include <string>
#include <ShObjIdl.h>

class NewPathSaveCode {
public:
    bool newPathSaveCode(std::wstring& t_Path);

private:
    void saveFile(const std::wstring& t_Path, std::ofstream& outFile);
    std::wstring BrowseFileDialog() const;

    std::wstring m_Path;
    const std::wstring m_PathOptionDirectory = L"dataAvLoad\\PathWar3.dat";
};

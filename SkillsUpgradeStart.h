#pragma once
#include <windows.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <gdiplus.h>

class SkillsUpgradeStart
{
public:
    SkillsUpgradeStart(const std::wstring& t_NameMaps, const int& t_VersionWarcraft3);
    void skillsUpgradeStart(const bool& isDelay = true);
private:
    void delayIsEndLoad();
    bool loadDataSkill();
    bool charToAscii(int& c);
    std::vector<std::pair<int, unsigned>> m_DataSkill;
    std::wstring m_FullPath = L"\0";
    const std::wstring& m_NameMaps;
    const std::wstring m_pathDatMap = L"DataMaps";
    const std::wstring m_pathFile = L"\\Skill.txt";
    const int& m_VersionWarcraft3;
    cv::Mat captureScreen();
};


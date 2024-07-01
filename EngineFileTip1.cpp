#include <Windows.h>
#include <iostream>
#include <filesystem>

#include "getMapOpen.h"
#include "LoadManager.h"
#include "EngineFileTip1.h"
#include "NewDirectory.h"
#include "SkillsUpgradeStart.h"


EngineFileTip1::EngineFileTip1(DataPath& t_DataPath, sf::RenderWindow& t_window, sf::Font& t_font, OptionsData& t_OptionsData)
    : m_DataPath(t_DataPath), window(t_window), font(t_font), m_OptionsData(t_OptionsData)
{
}

void EngineFileTip1::engineFile()
{
    getMapOpen getMapOpen_;
    m_FileName = getMapOpen_.getMapOpen1(m_DataPath.getMapsPath());
    if (m_FileName == L"Error") return;


    std::wstring str2 = m_DataPath.getSavePath();
    m_DataMaps.NameMaps(m_FileName);
    if (!m_DataMaps.PutSaveCode(str2)) {

        sf::Vector2f newPosition(0, 0);
        std::wstring wstr(m_DataPath.getSavePath());
        std::wstring wstr2(m_DataMaps.getNameMaps());
        bool faic = false;
        NewDirectory app(window, font, newPosition, wstr2, wstr, faic, m_OptionsData);
        app.newDirectory();

        return; 
    }

    bool isError = false;

    m_ListHero = m_DataPath.retrieveHeroData(m_DataMaps.getPutSaveCode(), isError);
    if (isError) return;


    engineTip1();

    return;
}

void EngineFileTip1::engineTip1()
{
    std::wstring patc;
    std::wstring nameChar;

    if (m_ListHero.size() > 1) {
        auto latestTime = std::filesystem::file_time_type();
        for (const auto& p : m_ListHero) {
            if (p.latestTime > latestTime) {
                latestTime = p.latestTime;
                patc = p.path;
                nameChar = p.nameChar;
            }
        }
    }
    else {
        patc = m_ListHero.front().path;
    }
    std::wcout << "Hero:  " << nameChar << std::endl;

    LoadManager LoadManager_;
    LoadManager_.executeLoad(patc);

    std::wstring nameMaps(m_DataMaps.getNameMaps());
    SkillsUpgradeStart ScilsUpgradeStart_(nameMaps);
    ScilsUpgradeStart_.skillsUpgradeStart();


    LoadComands LoadComands_(m_DataPath);
    Comands comands = LoadComands_.loadComands();

    std::string cmdStr = "\0";
    if (comands.returnCmd.isLoad) cmdStr = comands.returnCmd.str;
    else if (comands.saveCmd.isLoad) cmdStr = comands.saveCmd.str;
    else if (comands.craftCmd.isLoad) cmdStr = comands.craftCmd.str;
    else if (comands.camCmd.isLoad) cmdStr = comands.camCmd.str;
    else if (comands.statsCmd.isLoad) cmdStr = comands.statsCmd.str;
    else if (comands.clearCmd.isLoad) cmdStr = comands.clearCmd.str;
    else if (comands.cleanCmd.isLoad) cmdStr = comands.cleanCmd.str;
    else if (comands.strCmd.isLoad) cmdStr = comands.strCmd.str;
    else if (comands.agiCmd.isLoad) cmdStr = comands.agiCmd.str;
    else if (comands.intCmd.isLoad) cmdStr = comands.intCmd.str;

    LoadManager_.sendLoadCommands({ cmdStr }, true);
}



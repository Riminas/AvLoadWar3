#include <Windows.h>
#include <iostream>
#include <filesystem>

#include "getMapOpen.h"
#include "LoadManager.h"
#include "EngineFileTip1.h"
#include "NewDirectory.h"
#include "SkillsUpgradeStart.h"
#include "EngineDataCommands.h"


EngineFileTip1::EngineFileTip1(sf::RenderWindow& t_window, sf::Font& t_font, DataAll& t_DataAll)
    : m_Window(t_window), m_Font(t_font), m_DataAll(t_DataAll)
{
}

void EngineFileTip1::engineFile()
{
    bool isError = false;

    m_ListHero = m_DataAll.m_DataPath.retrieveHeroData(m_DataAll.m_DataMaps.getPutSaveCode(), isError);
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

    LoadManager LoadManager_(m_DataAll.m_DataPath.m_hWndWindow);
    LoadManager_.executeLoad(patc);

    if (m_DataAll.m_OptionsData.autoSkillsUpgrade) {
        const std::wstring& nameMaps(m_DataAll.m_DataMaps.getNameMaps());
        SkillsUpgradeStart ScilsUpgradeStart_(nameMaps, m_DataAll.versionWarcraft3);
        ScilsUpgradeStart_.skillsUpgradeStart();
    }

    EngineDataCommands EngineDataCommands_(m_DataAll.m_DataCommands);
    EngineDataCommands_.avtoCommand(m_DataAll.m_DataPath.m_hWndWindow);
}



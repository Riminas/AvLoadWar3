#include <Windows.h>
#include <iostream>
#include <filesystem>

#include "getMapOpen.h"
#include "LoadManager.h"
#include "EngineFileTip1.h"
#include "NewDirectory.h"
#include "SkillsUpgradeStart.h"
#include "LoadCommands.h"


EngineFileTip1::EngineFileTip1(sf::RenderWindow& t_window, sf::Font& t_font, DataAll& t_DataAll)
    : m_Window(t_window), m_Font(t_font), m_DataAll(t_DataAll)
{
}

void EngineFileTip1::engineFile()
{
    NewDataAll NewDataAll_(m_DataAll, m_Window, m_Font);
    NewDataAll_.newMaps(true);

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

    std::wstring nameMaps(m_DataAll.m_DataMaps.getNameMaps());
    SkillsUpgradeStart ScilsUpgradeStart_(nameMaps);
    ScilsUpgradeStart_.skillsUpgradeStart();

    std::string cmdStr = "\0";
    if (m_DataAll.m_Commands.returnCmd.isLoad) { cmdStr = m_DataAll.m_Commands.returnCmd.str; LoadManager_.sendLoadCommands({ cmdStr }, true); }
    if (m_DataAll.m_Commands.saveCmd.isLoad) { cmdStr = m_DataAll.m_Commands.saveCmd.str; LoadManager_.sendLoadCommands({ cmdStr }, true); }
    if (m_DataAll.m_Commands.craftCmd.isLoad) { cmdStr = m_DataAll.m_Commands.craftCmd.str; LoadManager_.sendLoadCommands({ cmdStr }, true); }
    if (m_DataAll.m_Commands.camCmd.isLoad) { cmdStr = m_DataAll.m_Commands.camCmd.str; LoadManager_.sendLoadCommands({ cmdStr }, true); }
    if (m_DataAll.m_Commands.statsCmd.isLoad) { cmdStr = m_DataAll.m_Commands.statsCmd.str; LoadManager_.sendLoadCommands({ cmdStr }, true); }
    if (m_DataAll.m_Commands.clearCmd.isLoad) { cmdStr = m_DataAll.m_Commands.clearCmd.str; LoadManager_.sendLoadCommands({ cmdStr }, true); }
    if (m_DataAll.m_Commands.cleanCmd.isLoad) { cmdStr = m_DataAll.m_Commands.cleanCmd.str; LoadManager_.sendLoadCommands({ cmdStr }, true); }
    if (m_DataAll.m_Commands.strCmd.isLoad) { cmdStr = m_DataAll.m_Commands.strCmd.str; LoadManager_.sendLoadCommands({ cmdStr }, true); }
    if (m_DataAll.m_Commands.agiCmd.isLoad) { cmdStr = m_DataAll.m_Commands.agiCmd.str; LoadManager_.sendLoadCommands({ cmdStr }, true); }
    if (m_DataAll.m_Commands.intCmd.isLoad) { cmdStr = m_DataAll.m_Commands.intCmd.str; LoadManager_.sendLoadCommands({ cmdStr }, true); }
}



#include "LoadManager.h"
#include "EngineDataCommands.h"

EngineDataCommands::EngineDataCommands(const DataCommands& t_DataCommands)
	: m_DataCommands(t_DataCommands)
{
}

void EngineDataCommands::avtoCommand(const HWND& hWndWindow)
{
    LoadManager LoadManager_(hWndWindow);
    std::string cmdStr;
    for (int i = 0; i < 11; i++) {
        if (isLoad(i)) {
            cmdStr = getStringCmd(i); 
            LoadManager_.sendLoadDataCommands({ cmdStr }, true); 
        }
    }
}


const std::string& EngineDataCommands::getStringCmd(const int& num) {
    switch (num)
    {
    case 0: return m_DataCommands.return1Cmd.str;
    case 1: return m_DataCommands.return2Cmd.str;
    case 2: return m_DataCommands.return3Cmd.str;
    case 4: return m_DataCommands.saveCmd.str;
    case 5: return m_DataCommands.craftCmd.str;
    case 6: return m_DataCommands.camCmd.str;
    case 7: return m_DataCommands.statsCmd.str;
    case 8: return m_DataCommands.clearCmd.str;
    case 9: return m_DataCommands.cleanCmd.str;
    case 10: return m_DataCommands.strCmd.str;
    case 11: return m_DataCommands.agiCmd.str;
    case 12: return m_DataCommands.intCmd.str;
    default: return "\0";
    }
}

bool EngineDataCommands::isVisibleButton(const int& num) {
    switch (num)
    {
    case 0: return m_DataCommands.return1Cmd.isVisibleButton;
    case 1: return m_DataCommands.return2Cmd.isVisibleButton;
    case 2: return m_DataCommands.return3Cmd.isVisibleButton;
    case 4: return m_DataCommands.saveCmd.isVisibleButton;
    case 5: return m_DataCommands.craftCmd.isVisibleButton;
    case 6: return m_DataCommands.camCmd.isVisibleButton;
    case 7: return m_DataCommands.statsCmd.isVisibleButton;
    case 8: return m_DataCommands.clearCmd.isVisibleButton;
    case 9: return m_DataCommands.cleanCmd.isVisibleButton;
    case 10: return m_DataCommands.strCmd.isVisibleButton;
    case 11: return m_DataCommands.agiCmd.isVisibleButton;
    case 12: return m_DataCommands.intCmd.isVisibleButton;
    default: return false;
    }
}

bool EngineDataCommands::isLoad(const int& num) {
    switch (num)
    {
    case 0: return m_DataCommands.return1Cmd.isLoad;
    case 1: return m_DataCommands.return2Cmd.isLoad;
    case 2: return m_DataCommands.return3Cmd.isLoad;
    case 4: return m_DataCommands.saveCmd.isLoad;
    case 5: return m_DataCommands.craftCmd.isLoad;
    case 6: return m_DataCommands.camCmd.isLoad;
    case 7: return m_DataCommands.statsCmd.isLoad;
    case 8: return m_DataCommands.clearCmd.isLoad;
    case 9: return m_DataCommands.cleanCmd.isLoad;
    case 10: return m_DataCommands.strCmd.isLoad;
    case 11: return m_DataCommands.agiCmd.isLoad;
    case 12: return m_DataCommands.intCmd.isLoad;
    default: return false;
    }
}
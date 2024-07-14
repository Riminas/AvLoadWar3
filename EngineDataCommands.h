#pragma once
#include <wtypes.h>
#include "DataCommands.h"
class EngineDataCommands
{
public:
    EngineDataCommands(const DataCommands& t_DataCommands);
    void avtoCommand(const HWND& hWndWindow);
    const std::string& getStringCmd(const int& num);
    bool isVisibleButton(const int& num);
    bool isLoad(const int& num);
private:
    const DataCommands& m_DataCommands;
};


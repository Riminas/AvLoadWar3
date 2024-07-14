#pragma once
#include "DataPath.h"
#include "DataMaps.h"
#include "OptionsData.h"
#include "DataCommands.h"
#include <wtypes.h>

class DataAll
{
public:
    int versionWarcraft3 = 0;

    DataPath m_DataPath;
    DataMaps m_DataMaps;
    OptionsData m_OptionsData;
    DataCommands m_DataCommands;

    bool isNewMaps = false;
    bool isNewWarcrft = false;
    bool isVisibleWindow = true;
    bool isMapsStart = false;
    bool m_IsUpdataOwnerWindow = false;
private:
};

#include <SFML/Graphics.hpp>

class NewDataAll
{
public:
	NewDataAll(DataAll& t_DataAll, sf::RenderWindow& t_Window, sf::Font& t_Font);

    void newMaps(const bool& isNoDataCommands = false, const bool& isOptions = false);
    void newWarcrft(HWND& hWndWindow);
private:

    DataAll& m_DataAll;
    sf::RenderWindow& m_Window;
    sf::Font& m_Font;
};
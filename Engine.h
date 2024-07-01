#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "DataPath.h"
#include "DataMaps.h"
#include "OwnerWindow.h"
#include "EngineFileTip2.h"
#include "OptionsData.h"

class Engine
{
public:
    Engine();
    void engine1();

private:
    bool IsWindowInFocus(HWND hWnd);
    void updateWindowVisibility();
    void processEvents();
    void draw();
    void handleMouseButtonPressed(sf::Event& event);
    void isInitialize(bool lastIsActiveWindow[]);

    bool initialize();

    void initializeTree();
    void MinimizeToTray();

    void checkKeyStates();

    sf::RenderWindow m_Window;

    sf::Font m_Font;
    DataPath m_DataPath;
    DataMaps m_DataMaps;
    OwnerWindow m_OwnerWindow;
    EngineFileTip2 m_EngineFileTip2;
    sf::Clock m_Clock;
    HWND m_hWndWindow = NULL;
    bool m_IsActiveWindow[1] = {false};

    bool m_IsVisibleLoad = false;
    bool m_IsLastActive = true;
    bool m_IsVisibleOwnerWindow = true;
    bool m_IsWar126 = false;

    std::unordered_map<int, std::chrono::high_resolution_clock::time_point> keyPressTimes;
    int currentKey = -1;  // Track the currently pressed key
    OptionsData m_OptionsData;
};

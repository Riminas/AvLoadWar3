#include <Windows.h>

#include <wtypes.h>

#include <iostream>

#include "CoutButtonUser.h"
#include "OwnerWindow.h"
#include "LoadDataFail.h"
#include "LoadManager.h"
#include "EngineFileTip1.h"
#include "getMapOpen.h"
#include "NewDirectory.h"
#include "EngineDataCommands.h"
#include "GlobalVarible.h"
#include "SkillsUpgradeStart.h"

OwnerWindow::OwnerWindow(sf::RenderWindow& t_Window, sf::Font& t_Font, DataAll& t_DataAll, bool& t_isExetTree, bool& t_IsVisibleLoad)
    : m_Window(t_Window),
    m_Font(t_Font),
    m_DataAll(t_DataAll),
    isExetTree(t_isExetTree),
    m_IsVisibleLoad(t_IsVisibleLoad)
{
}

void OwnerWindow::draw() {

    if (m_IsVisibleLoad) {
        m_Window.draw(shapeIsLoad);
        m_Window.draw(textureIsLoad);
        return;
    }

    if (!m_IsVisibleOwnerWindow)
        return;

    if (!m_IsVisibleMainMenu) {
        m_Window.draw(m_ShapeFalseVisibleMainMenu);
        return;
    }

    for (const Button& button : m_Buttons) {
        if (button.isVisibleButton) {
            m_Window.draw(button.shape);
            if (button.isLoadTextur)
                m_Window.draw(button.sprite);
            else
                m_Window.draw(button.text);
        }
    }

    if (m_IsVisibleLoad) {
        m_Window.draw(shapeIsLoad);
        m_Window.draw(textureIsLoad);
    }

    if (!m_IsVisibleMenu)
        return;

    for (const Button& button : m_ButtonsMenu) {
        m_Window.draw(button.shape);
        m_Window.draw(button.sprite);
    }

    m_Window.draw(m_ShapeTrueVisibleMainMenu);
    
    //const std::string strHablon = "dataAvLoad\\hablon\\hablonUser.png";
    //CoutButtonUser CoutButtonUser_;
    //const sf::Vector2f result = CoutButtonUser_.findElementInImage(strHablon);
    //if (result.x != -1 && result.y != -1) {
        //m_ShapePlayerName.shape.setPosition(sf::Vector2f(result.x, result.y) + sf::Vector2f(100, 100));
        //m_ShapePlayerName.sprite.setPosition(m_ShapePlayerName.shape.getPosition() + sf::Vector2f(2, 2));

    m_Window.draw(m_ShapePlayerName.shape);
    m_Window.draw(m_ShapePlayerName.sprite);
    //}
}

void OwnerWindow::processingButton(const sf::Vector2i& event, bool isWindow2Visible[]) {
    if (!m_IsVisibleOwnerWindow)
        return;

    const int numButton = mouseButtonPressed(event, isWindow2Visible);
    
    if (numButton == -5) {
        std::wstring PlayerName = L"\0";
        const std::wstring pathFull = L"dataAvLoad//PlayerName.ini";
        if (!std::filesystem::exists(pathFull)) {
            std::ofstream fil(pathFull);
            fil << "PlayerName";
            fil.close();
            PlayerName = L"PlayerName";
        }
        else {
            LoadDataFail LoadDataFail_;
            PlayerName = LoadDataFail_.loadDataFail(pathFull);
        }
        if (!PlayerName.empty()) {
            StringConvector StringConvector_;
            LoadManager LoadManager_(m_DataAll.m_DataPath.m_hWndWindow);
            LoadManager_.sendLoadDataCommands({ StringConvector_.utf16_to_utf8(PlayerName) }, false);
        }
    }
    else if (numButton == -4) {
        m_IsVisibleMainMenu = true;
    }
    else if (numButton == -3) {
        m_IsVisibleMainMenu = false;
    }
    else if (numButton == 3) {
        m_IsVisibleMenu = !m_IsVisibleMenu;
    }
    else if (numButton >= 0) {
        //if (m_DataAll.m_DataCommands.return1Cmd.str != "False") {

            EngineDataCommands EngineDataCommands_(m_DataAll.m_DataCommands);
            const std::string str = EngineDataCommands_.getStringCmd(numButton);
            
            LoadManager LoadManager_(m_DataAll.m_DataPath.m_hWndWindow);
            LoadManager_.sendLoadDataCommands({ str }, false);
        //}
    }
}

int OwnerWindow::mouseButtonPressed(const sf::Vector2i& event, bool isWindow2Visible[])
{
    for (int i = 0; i < m_Buttons.size(); ++i) {
        if (m_Buttons[i].shape.getGlobalBounds().contains(static_cast<float>(event.x), static_cast<float>(event.y+2))) {
             return i;
        }
    }

    if (m_ShapeTrueVisibleMainMenu.getGlobalBounds().contains(static_cast<float>(event.x), static_cast<float>(event.y + 2))) {
        return -3;
    }
    else if (m_ShapeFalseVisibleMainMenu.getGlobalBounds().contains(static_cast<float>(event.x), static_cast<float>(event.y + 2))) {
        return -4;
    }
    else if (m_ShapePlayerName.shape.getGlobalBounds().contains(static_cast<float>(event.x), static_cast<float>(event.y + 2))) {
        return -5;
    }

    return -2;
}

void OwnerWindow::processingButtonMenu(const sf::Vector2i& event, bool isWindow2Visible[]) {
    if (!m_IsVisibleMenu || !m_IsVisibleOwnerWindow)
        return;

    const int numButton = mouseButtonMenuPressed(event, isWindow2Visible);
    
    if (numButton == -2)
        return;
    else if (numButton == -1) {
        m_Window.close();
        return;
    }

    if (!m_DataAll.isNewMaps && numButton <= 1) {
        NewDataAll NewDataAll_(m_DataAll, m_Window, m_Font);
        NewDataAll_.newMaps(true);

        m_Window.clear(sf::Color(0, 255, 0));

        m_Window.display();

        updateButtonsVisible();

        if (!m_DataAll.isNewMaps)
            return;
    }

    if (numButton == 0) {
        if (isWindow2Visible[0]) {
            isWindow2Visible[0] = false;
        }
       /* const std::wstring wstr = m_DataAll.m_DataMaps.getPutSaveCode();
        if (wstr.empty()) {
            LoadManager LoadManager_(m_DataAll.m_DataPath.m_hWndWindow);
            LoadManager_.sendLoadDataCommands({ "-Load" }, false);
        }*/

        m_IsVisibleMenu = !m_IsVisibleMenu;

        m_IsVisibleLoad = true;

        m_Window.clear(sf::Color(0, 255, 0));
        draw();
        m_Window.display();
        
        EngineFileTip1 EngineFileTip1_(m_Window, m_Font, m_DataAll);
        EngineFileTip1_.engineFile();

        m_IsVisibleLoad = false;

        if (m_DataAll.m_OptionsData.autoSave) {
            m_DataAll.m_OptionsData.isStaitAuto = true;
            m_DataAll.m_OptionsData.lastSaveTime = std::chrono::high_resolution_clock::now();
        }
    }
    else if (numButton == 1) {
        isWindow2Visible[0] = !isWindow2Visible[0];
    }
    else if (numButton == 2) {
        G_IS_ACTIVITI_HOT_KEY = false;
        NewDataAll NewDataAll_(m_DataAll, m_Window, m_Font);
        NewDataAll_.newMaps(true, true);

        m_IsVisibleMenu = !m_IsVisibleMenu;

        m_Window.clear(sf::Color(0, 255, 0));

        m_Window.display();

        updateButtonsVisible();
        G_IS_ACTIVITI_HOT_KEY = true;
    }
    //else if (numButton == 3) {

    //    m_IsVisibleMenu = false;
    //    m_IsVisibleLoad = true;


    //    m_Window.clear(sf::Color(0, 255, 0));
    //    draw();
    //    m_Window.display();

    //    const std::wstring& nameMaps(m_DataAll.m_DataMaps.getNameMaps());
    //    SkillsUpgradeStart ScilsUpgradeStart_(nameMaps, m_DataAll.versionWarcraft3);
    //    ScilsUpgradeStart_.skillsUpgradeStart(false);

    //    m_IsVisibleLoad = false;
    //}
}

int OwnerWindow::mouseButtonMenuPressed(const sf::Vector2i& event, bool isWindow2Visible[])
{
    for (int i = 0; i < m_ButtonsMenu.size(); ++i) {
        if (m_ButtonsMenu[i].shape.getGlobalBounds().contains(static_cast<float>(event.x), static_cast<float>(event.y + 2))) {
            if (i == 0) {
                return 0;
            }
            else if (i == 1) {
                return 1;
            }
            else if (i == 2) {
                return 2;
            }
            //else if (i == 3) {
            //    return 3;
            //}
            else if (i == 3) {
                return -1;
            }
        }
    }

    return -2;
}

void OwnerWindow::updateButtonsVisible() {
    EngineDataCommands EngineDataCommands_(m_DataAll.m_DataCommands);
    const int maxButton = static_cast<int>(m_Buttons.size());
    for (int i = 0; i < maxButton; ++i) {
        if (i != 3) {
            //if(!m_DataAll.isNewMaps)
            //    m_Buttons[i].isVisibleButton = false;
            //else
                m_Buttons[i].isVisibleButton = EngineDataCommands_.isVisibleButton(i);
        }
        else {
            m_Buttons[i].isVisibleButton = true;
        }
    }
}

void OwnerWindow::initializeButtonsDataCommands() {

    m_Window.clear(sf::Color(0, 255, 0));

    m_Window.display();

    //if (!m_DataCommands.load)
    //    return;
    const std::vector<std::string> buttonTextures = { 
        "dataAvLoad\\img\\return.png",
        "dataAvLoad\\img\\return.png",
        "dataAvLoad\\img\\return.png",
        "dataAvLoad\\img\\menu.png",
        "dataAvLoad\\img\\save.png",
        "dataAvLoad\\img\\craft.png",
        "dataAvLoad\\img\\cam.png",
        "dataAvLoad\\img\\stats.png",
        "dataAvLoad\\img\\clear.png",
        "dataAvLoad\\img\\clean.png",
        "dataAvLoad\\img\\str.png",
        "dataAvLoad\\img\\agi.png",
        "dataAvLoad\\img\\int.png" };

    //const std::vector<std::string> buttonTextures = {  };

    int minus = 0;
    m_Buttons.resize(buttonTextures.size());
    EngineDataCommands EngineDataCommands_(m_DataAll.m_DataCommands);
    const int maxButton = static_cast<int>(m_Buttons.size());
    for (int i = 0; i < maxButton; ++i) {
        m_Buttons[i].shape.setSize(sf::Vector2f(static_cast<float>(m_ConstSize.x - 1), static_cast<float>(m_ConstSize.x - 1)));

        if (i != 3)
            m_Buttons[i].isVisibleButton = EngineDataCommands_.isVisibleButton(i);
        else
            m_Buttons[i].isVisibleButton = true;

        m_Buttons[i].shape.setPosition(sf::Vector2f(2, static_cast<float>((i - minus) * m_ConstSize.x + 2) - 100.0f));  // Размещаем квадраты по вертикали

        m_Buttons[i].shape.setOutlineColor(sf::Color::Black);
        m_Buttons[i].shape.setOutlineThickness(2);

        // Load texture
        if (!m_Buttons[i].texture.loadFromFile(buttonTextures[i])) {
            m_Buttons[i].isLoadTextur = false;

            const std::vector<std::string> buttonTextures = { "re1", "re2", "re3", "men",  "sav", "cra", "cam", "sta", "clr", "cln", "str", "agi", "int" };

            m_Buttons[i].text.setFont(m_Font);
            m_Buttons[i].text.setFillColor(sf::Color::Black);
            m_Buttons[i].text.setCharacterSize(12);
            m_Buttons[i].text.setString(buttonTextures[i]);
            m_Buttons[i].text.setRotation(45.0f);
            m_Buttons[i].text.setLetterSpacing(1.5f);
            m_Buttons[i].text.setStyle(1);
        }

        // Set texture to sprite
        m_Buttons[i].sprite.setTexture(m_Buttons[i].texture);
        m_Buttons[i].sprite.setPosition(m_Buttons[i].shape.getPosition() + sf::Vector2f(2, 2));
        m_Buttons[i].sprite.setScale(.12f, .12f);

    }
}


void OwnerWindow::initialize() {

    setupWindow();

    initializeButtonsDataCommands();

    const std::vector<std::string> buttonMenuTextures = { 
        "dataAvLoad\\img\\upload.png", 
        "dataAvLoad\\img\\upLoadAll.png", 
        "dataAvLoad\\img\\options.png",
        //"dataAvLoad\\img\\avskills.png",
        "dataAvLoad\\img\\exit.png",};
    //const std::vector<std::string> buttonTextures = {  };
    m_ButtonsMenu.resize(buttonMenuTextures.size());
    const int maxButtonMenu = static_cast<int>(m_ButtonsMenu.size());
    for (int i = 0; i < maxButtonMenu; ++i) {

        m_ButtonsMenu[i].shape.setSize(sf::Vector2f(static_cast<float>(m_ConstSize.x-1), static_cast<float>(m_ConstSize.x-1)));

        //m_ButtonsMenu[i].shape.setPosition(sf::Vector2f(2, static_cast<float>(i * m_ConstSize.x + 2) - 100.0f));  // Размещаем квадраты по вертикали

        m_ButtonsMenu[i].shape.setOutlineColor(sf::Color::Black);
        m_ButtonsMenu[i].shape.setOutlineThickness(2);

        // Load texture
        if (!m_ButtonsMenu[i].texture.loadFromFile(buttonMenuTextures[i])) {
            // Handle error
        }

        // Set texture to sprite
        m_ButtonsMenu[i].sprite.setTexture(m_ButtonsMenu[i].texture);
        //m_ButtonsMenu[i].sprite.setPosition(m_ButtonsMenu[i].shape.getPosition() + sf::Vector2f(2, 2));
        m_ButtonsMenu[i].sprite.setScale(.12f, .12f);
        
    }

    shapeIsLoad.setFillColor(sf::Color::White);
    shapeIsLoad.setSize(sf::Vector2f(200, 25));

    textureIsLoad.setFont(m_Font);
    textureIsLoad.setCharacterSize(16);
    textureIsLoad.setString("Loading...");
    textureIsLoad.setFillColor(sf::Color::Black);

    //m_ShapeTrueVisibleMainMenu;
    m_ShapeTrueVisibleMainMenu.setSize(sf::Vector2f(static_cast<float>(m_ConstSize.x - 1), static_cast<float>(m_ConstSize.x - 1)));
    m_ShapeTrueVisibleMainMenu.setOutlineColor(sf::Color::Black);
    m_ShapeTrueVisibleMainMenu.setOutlineThickness(2);

    //m_ShapeFalseVisibleMainMenu;
    m_ShapeFalseVisibleMainMenu.setSize(sf::Vector2f(10.f, 10.f));
    m_ShapeFalseVisibleMainMenu.setOutlineColor(sf::Color::Black);
    m_ShapeFalseVisibleMainMenu.setOutlineThickness(2);

    //m_ShapePlayerName;
    m_ShapePlayerName.shape.setSize(sf::Vector2f(static_cast<float>(m_ConstSize.x - 1), static_cast<float>(m_ConstSize.x - 1)));
    m_ShapePlayerName.shape.setOutlineColor(sf::Color::Black);
    m_ShapePlayerName.shape.setOutlineThickness(2);
   
    m_ShapePlayerName.texture.loadFromFile("dataAvLoad\\img\\user.png"); //user.png
    // Set texture to sprite
    m_ShapePlayerName.sprite.setTexture(m_ShapePlayerName.texture);
    //m_ButtonsMenu[i].sprite.setPosition(m_ButtonsMenu[i].shape.getPosition() + sf::Vector2f(2, 2));
    m_ShapePlayerName.sprite.setScale(.12f, .12f);
}

// Функция для установки окна поверх всех других окон
void setAlwaysOnTop(HWND hwnd) {
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}

void OwnerWindow::setupWindow() {
    const unsigned xW = sf::VideoMode::getDesktopMode().width - 2;
    const unsigned yW = sf::VideoMode::getDesktopMode().height - 2;
    m_Window.create(sf::VideoMode(xW, yW), "AvLoads", sf::Style::None);

    {
        HWND hwnd = m_Window.getSystemHandle();

        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_NOACTIVATE);
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        SetLayeredWindowAttributes(hwnd, RGB(0, 255, 0), 0, LWA_COLORKEY);
    }


    m_Window.setPosition(sf::Vector2i(1,1));
    m_Window.setVerticalSyncEnabled(true);


    // Получаем дескриптор окна
    HWND hwnd = m_Window.getSystemHandle();

    // Устанавливаем окно поверх всех других окон
    setAlwaysOnTop(hwnd);

    // Запускаем таймер в отдельном потоке для периодического обновления окна
    std::thread([hwnd]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            setAlwaysOnTop(hwnd);
        }
        }).detach();
}

void OwnerWindow::activeGameTrue(const HWND& hWndWindow) {
    m_IsVisibleOwnerWindow = true;

    m_Window.setVerticalSyncEnabled(true);

    if (m_DataAll.m_IsUpdataOwnerWindow) {
        m_DataAll.m_IsUpdataOwnerWindow = false;
        updateButtonsVisible();
    }

    RECT rectClient;
    RECT rectWindow;
    if (GetClientRect(hWndWindow, &rectClient) && GetWindowRect(hWndWindow, &rectWindow)) {
        const int width = rectClient.right - rectClient.left;
        const int height = rectClient.bottom - rectClient.top;
        if (height < 300 || width < 300) {
            m_DataAll.isNewWarcrft = false;
            m_IsVisibleOwnerWindow = false;
            return;
        }
        const float x = width / 2.f - (static_cast<float>(m_Buttons.size()) * m_ConstSize.x) / 3 + 100;
        const float y = (height / 20.0f) * 15.75f - 10;

        sf::Vector2f newPosition = sf::Vector2f(x, y);
        const sf::Vector2f windowPoition = sf::Vector2f(rectWindow.left, rectWindow.top);
        const sf::Vector2f windowWidthHeight = sf::Vector2f(width, height);
        updatePosition(newPosition, windowPoition, windowWidthHeight);
    }
}

void OwnerWindow::activeGameFalse() {
    m_IsVisibleOwnerWindow = false;

    m_Window.setVerticalSyncEnabled(false);
    m_Window.setFramerateLimit(10);
}


void OwnerWindow::updatePosition(const sf::Vector2f& newPoition, const sf::Vector2f& windowPoition, const sf::Vector2f& windowWidthHeight)
{
    shapeIsLoad.setPosition(sf::Vector2f(windowWidthHeight.x / 2 - shapeIsLoad.getSize().x / 2, windowWidthHeight.y / 2 - shapeIsLoad.getSize().y / 2));
    textureIsLoad.setPosition(shapeIsLoad.getPosition() + sf::Vector2f(8, 3));

    m_ShapeFalseVisibleMainMenu.setPosition(sf::Vector2f(0.f, 0.f) + windowPoition);


    int minus = 0;
    for (unsigned i = 0; Button & button : m_Buttons) {
        if (!button.isVisibleButton)
            minus++;
        button.shape.setPosition(sf::Vector2f(static_cast<float>((i - minus) * m_ConstSize.x + 2) - 100.0f, 2) + newPoition + windowPoition);
        button.sprite.setPosition(button.shape.getPosition() + sf::Vector2f(2, 2));
        button.text.setPosition(button.shape.getPosition() + sf::Vector2f(8.f, -4.f));

        i++;
    }
    const float x = m_Buttons[3].shape.getPosition().x;
    for (unsigned i = 0; Button& buttonMenu : m_ButtonsMenu) {

        buttonMenu.shape.setPosition(sf::Vector2f(x, static_cast<float>(i * m_ConstSize.x + 2) - 5.0f - (m_ButtonsMenu.size() * m_ConstSize.x) + newPoition.y + windowPoition.y));
        buttonMenu.sprite.setPosition(buttonMenu.shape.getPosition() + sf::Vector2f(2, 2));

        i++;
    }

    m_ShapeTrueVisibleMainMenu.setPosition(sf::Vector2f(x, static_cast<float>(m_ConstSize.x + 2) + 5.0f + newPoition.y + windowPoition.y));

    m_ShapePlayerName.shape.setPosition(m_ShapeTrueVisibleMainMenu.getPosition() + sf::Vector2f(0, m_ConstSize.x));
    m_ShapePlayerName.sprite.setPosition(m_ShapePlayerName.shape.getPosition() + sf::Vector2f(2, 2));
}



void OwnerWindow::setIsVisibleMenu(const bool& t_IsVisibleMenu) { m_IsVisibleMenu = t_IsVisibleMenu; }


#include <wtypes.h>

#include "OwnerWindow.h"
#include <iostream>
#include "LoadCommands.h"
#include "LoadManager.h"
#include "EngineFileTip1.h"
#include "getMapOpen.h"
#include "NewDirectory.h"

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

    for (const Button & button : m_Buttons) {
        if (button.isVisibleButton) {
            m_Window.draw(button.shape);
            if(button.isLoadTextur)
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

    for (const Button & button : m_ButtonsMenu) {
        m_Window.draw(button.shape);
        m_Window.draw(button.sprite);
    }
}

void OwnerWindow::processingButton(const sf::Vector2i& event, bool isWindow2Visible[]) {
    if (!m_IsVisibleOwnerWindow)
        return;

    const int numButton = mouseButtonPressed(event, isWindow2Visible);
    
    if (numButton == 1) {
        m_IsVisibleMenu = !m_IsVisibleMenu;
    }
    else if (numButton == 0 || numButton >= 2) {
        NewDataAll NewDataAll_(m_DataAll, m_Window, m_Font);
        NewDataAll_.newMaps();
        if (m_DataAll.m_Commands.returnCmd.str != "False") {
            std::string str = "\0";
            switch (numButton)
            {
            case 0:
                str = m_DataAll.m_Commands.returnCmd.str;
                break;
            case 2:
                str = m_DataAll.m_Commands.saveCmd.str;
                break;
            case 3:
                str = m_DataAll.m_Commands.craftCmd.str;
                break;
            case 4:
                str = m_DataAll.m_Commands.camCmd.str;
                break;
            case 5:
                str = m_DataAll.m_Commands.statsCmd.str;
                break;
            case 6:
                str = m_DataAll.m_Commands.clearCmd.str;
                break;
            case 7:
                str = m_DataAll.m_Commands.cleanCmd.str;
                break;
            case 8:
                str = m_DataAll.m_Commands.strCmd.str;
                break;
            case 9:
                str = m_DataAll.m_Commands.agiCmd.str;
                break;
            case 10:
                str = m_DataAll.m_Commands.intCmd.str;
                break;
            default:
                break;
            }
            LoadManager LoadManager_(m_DataAll.m_DataPath.m_hWndWindow);
            LoadManager_.sendLoadCommands({ str }, false);
        }
    }
}

int OwnerWindow::mouseButtonPressed(const sf::Vector2i& event, bool isWindow2Visible[])
{
    for (int i = 0; i < m_Buttons.size(); ++i) {
        if (m_Buttons[i].shape.getGlobalBounds().contains(static_cast<float>(event.x), static_cast<float>(event.y+2))) {
             return i;
        }
    }

    return -2;
}

void OwnerWindow::processingButtonMenu(const sf::Vector2i& event, bool isWindow2Visible[]) {
    if (!m_IsVisibleMenu || !m_IsVisibleOwnerWindow)
        return;

    const int numButton = mouseButtonMenuPressed(event, isWindow2Visible);
    
    if (numButton == -1) {
        m_Window.close();
        return;
    }
    else if (numButton == 0) {
        if (isWindow2Visible[0]) {
            isWindow2Visible[0] = false;
        }

        m_IsVisibleMenu = !m_IsVisibleMenu;

        m_IsVisibleLoad = true;

        m_Window.clear(sf::Color(0, 255, 0));

        draw();

        m_Window.display();

        EngineFileTip1 EngineFileTip1_(m_Window, m_Font, m_DataAll);
        EngineFileTip1_.engineFile();

        m_IsVisibleLoad = false;
    }
    else if (numButton == 1) {
        isWindow2Visible[0] = !isWindow2Visible[0];
    }
    else if (numButton == 2) {

        NewDataAll NewDataAll_(m_DataAll, m_Window, m_Font);
        NewDataAll_.newMaps(true, true);

        m_IsVisibleMenu = !m_IsVisibleMenu;

        m_Window.clear(sf::Color(0, 255, 0));

        m_Window.display();

        initializeButtonsCommands();
    }
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
            else if (i == 3) {
                return -1;
            }
        }
    }

    return -2;
}

void OwnerWindow::initializeButtonsCommands() {

    m_Window.clear(sf::Color(0, 255, 0));

    m_Window.display();

    LoadCommands LoadCommands_(m_DataAll);
    LoadCommands_.loadCommands();
    //if (!m_Commands.load)
    //    return;
    const std::vector<std::string> buttonTextures = { 
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

    int enumButton = 0;
    int minus = 0;
    m_Buttons.resize(buttonTextures.size());
    const int maxButton = static_cast<int>(m_Buttons.size());
    for (int i = 0; i < maxButton; ++i) {
        m_Buttons[i].shape.setSize(sf::Vector2f(static_cast<float>(m_ConstSize.x - 1), static_cast<float>(m_ConstSize.x - 1)));

        bool isVisibleButton = true;
        switch (i)
        {
        case 0:
            isVisibleButton = m_DataAll.m_Commands.returnCmd.isVisibleButton;
            enumButton++;
            break;
        case 2:
            isVisibleButton = m_DataAll.m_Commands.saveCmd.isVisibleButton;
            enumButton++;
            break;
        case 3:
            isVisibleButton = m_DataAll.m_Commands.craftCmd.isVisibleButton;
            enumButton++;
            break;
        case 4:
            isVisibleButton = m_DataAll.m_Commands.camCmd.isVisibleButton;
            enumButton++;
            break;
        case 5:
            isVisibleButton = m_DataAll.m_Commands.statsCmd.isVisibleButton;
            enumButton++;
            break;
        case 6:
            isVisibleButton = m_DataAll.m_Commands.clearCmd.isVisibleButton;
            enumButton++;
            break;
        case 7:
            isVisibleButton = m_DataAll.m_Commands.cleanCmd.isVisibleButton;
            enumButton++;
            break;
        case 8:
            isVisibleButton = m_DataAll.m_Commands.strCmd.isVisibleButton;
            enumButton++;
            break;
        case 9:
            isVisibleButton = m_DataAll.m_Commands.agiCmd.isVisibleButton;
            enumButton++;
            break;
        case 10:
            isVisibleButton = m_DataAll.m_Commands.intCmd.isVisibleButton;
            enumButton++;
            break;
        default:
            break;
        }

        //if (!isVisibleButton)
        if (i != 1)
            m_Buttons[i].isVisibleButton = isVisibleButton;
        else
            m_Buttons[i].isVisibleButton = true;

        m_Buttons[i].shape.setPosition(sf::Vector2f(2, static_cast<float>((i - minus) * m_ConstSize.x + 2) - 100.0f));  // Размещаем квадраты по вертикали

        m_Buttons[i].shape.setOutlineColor(sf::Color::Black);
        m_Buttons[i].shape.setOutlineThickness(2);

        // Load texture
        if (!m_Buttons[i].texture.loadFromFile(buttonTextures[i])) {
            m_Buttons[i].isLoadTextur = false;

            const std::vector<std::string> buttonTextures = {
                "men",
                "ret",
                "sav",
                "cra",
                "cam",
                "sta",
                "clr",
                "cln",
                "str",
                "agi",
                "int" };

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

    initializeButtonsCommands();

    const std::vector<std::string> buttonMenuTextures = { 
        "dataAvLoad\\img\\upload.png", 
        "dataAvLoad\\img\\upLoadAll.png", 
        "dataAvLoad\\img\\options.png",
        "dataAvLoad\\img\\exit.png",};
    //const std::vector<std::string> buttonTextures = {  };
    m_ButtonsMenu.resize(buttonMenuTextures.size());
    const int maxButtonMenu = static_cast<int>(m_ButtonsMenu.size());
    for (int i = 0; i < maxButtonMenu; ++i) {

        m_ButtonsMenu[i].shape.setSize(sf::Vector2f(static_cast<float>(m_ConstSize.x-1), static_cast<float>(m_ConstSize.x-1)));

        m_ButtonsMenu[i].shape.setPosition(sf::Vector2f(2, static_cast<float>(i * m_ConstSize.x + 2) - 100.0f));  // Размещаем квадраты по вертикали

        m_ButtonsMenu[i].shape.setOutlineColor(sf::Color::Black);
        m_ButtonsMenu[i].shape.setOutlineThickness(2);

        // Load texture
        if (!m_ButtonsMenu[i].texture.loadFromFile(buttonMenuTextures[i])) {
            // Handle error
        }

        // Set texture to sprite
        m_ButtonsMenu[i].sprite.setTexture(m_ButtonsMenu[i].texture);
        m_ButtonsMenu[i].sprite.setPosition(m_ButtonsMenu[i].shape.getPosition() + sf::Vector2f(2, 2));
        m_ButtonsMenu[i].sprite.setScale(.12f, .12f);
        
    }

    shapeIsLoad.setFillColor(sf::Color::White);
    shapeIsLoad.setSize(sf::Vector2f(200, 25));

    textureIsLoad.setFont(m_Font);
    textureIsLoad.setCharacterSize(16);
    textureIsLoad.setString("Loading...");
    textureIsLoad.setFillColor(sf::Color::Black);
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


    RECT rect;
    if (GetClientRect(hWndWindow, &rect)) {
        const int width = rect.right - rect.left;
        const int height = rect.bottom - rect.top;
        const float x = width/2 - (m_Buttons.size()*m_ConstSize.x)/3+100;
        const float y = (height / 20.0f) * 15.75f - 10;

        shapeIsLoad.setPosition(sf::Vector2f(width/2 - shapeIsLoad.getSize().x/2, height / 2 - shapeIsLoad.getSize().y / 2));
        textureIsLoad.setPosition(shapeIsLoad.getPosition() + sf::Vector2f(8,3));

        sf::Vector2f newPosition = sf::Vector2f(x, y);
        updatePosition(newPosition);
    }
}

void OwnerWindow::activeGameFalse() {
    m_IsVisibleOwnerWindow = false;

    m_Window.setVerticalSyncEnabled(false);
    m_Window.setFramerateLimit(10);
}


void OwnerWindow::updatePosition(const sf::Vector2f& newPoition)
{
    int minus = 0;
    for (unsigned i = 0; Button & button : m_Buttons) {
        if (!button.isVisibleButton)
            minus++;
        button.shape.setPosition(sf::Vector2f(static_cast<float>((i - minus) * m_ConstSize.x + 2) - 100.0f, 2) + newPoition);
        button.sprite.setPosition(button.shape.getPosition() + sf::Vector2f(2, 2));
        button.text.setPosition(button.shape.getPosition() + sf::Vector2f(8.f, -4.f));

        i++;
    }
    const float x = m_Buttons[1].shape.getPosition().x;
    for (unsigned i = 0; Button& buttonMenu : m_ButtonsMenu) {

        buttonMenu.shape.setPosition(sf::Vector2f(x, static_cast<float>(i * m_ConstSize.x + 2) - 85.0f + newPoition.y) );
        buttonMenu.sprite.setPosition(buttonMenu.shape.getPosition() + sf::Vector2f(2, 2));

        i++;
    }
}

void OwnerWindow::setIsVisibleMenu(const bool& t_IsVisibleMenu) { m_IsVisibleMenu = t_IsVisibleMenu; }


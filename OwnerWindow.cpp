#include <wtypes.h>

#include "OwnerWindow.h"
#include <iostream>
#include "LoadComands.h"
#include "LoadManager.h"
#include "EngineFileTip1.h"
#include "getMapOpen.h"
#include "NewDirectory.h"

OwnerWindow::OwnerWindow(sf::RenderWindow& t_window, sf::Font& t_Font, DataPath& t_DataPath, bool& t_isExetTree, bool& t_IsVisibleLoad, OptionsData& t_OptionsData)
    : m_Window(t_window),
    m_Font(t_Font),
    m_DataPath(t_DataPath),
    isExetTree(t_isExetTree),
    m_IsVisibleLoad(t_IsVisibleLoad),
    m_OptionsData(t_OptionsData)
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
            m_Window.draw(button.sprite);
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
    
    if (numButton == 0) {
        m_IsVisibleMenu = !m_IsVisibleMenu;
    }
    else if (numButton >= 1) {
        if (!m_DataPath.initializePaths()) {
            return;
        }
        LoadComands LoadComands_(m_DataPath);
        m_Comands = LoadComands_.loadComands();
        if (m_Comands.returnCmd.str != "False") {
            std::string str = "\0";
            switch (numButton)
            {
            case 1:
                str = m_Comands.returnCmd.str;
                break;
            case 2:
                str = m_Comands.saveCmd.str;
                break;
            case 3:
                str = m_Comands.craftCmd.str;
                break;
            case 4:
                str = m_Comands.camCmd.str;
                break;
            case 5:
                str = m_Comands.statsCmd.str;
                break;
            case 6:
                str = m_Comands.clearCmd.str;
                break;
            case 7:
                str = m_Comands.cleanCmd.str;
                break;
            case 8:
                str = m_Comands.strCmd.str;
                break;
            case 9:
                str = m_Comands.agiCmd.str;
                break;
            case 10:
                str = m_Comands.intCmd.str;
                break;
            default:
                break;
            }
            LoadManager LoadManager_;
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
        if (!m_DataPath.initializePaths()) {
            return;
        }

        if (isWindow2Visible[0]) {
            isWindow2Visible[0] = false;
        }

        m_IsVisibleMenu = !m_IsVisibleMenu;

        m_IsVisibleLoad = true;

        m_Window.clear(sf::Color(0, 255, 0));

        draw();

        m_Window.display();

        EngineFileTip1 EngineFileTip1_(m_DataPath, m_Window, m_Font, m_OptionsData);
        EngineFileTip1_.engineFile();

        m_IsVisibleLoad = false;
    }
    else if (numButton == 1) {
        if (!m_DataPath.initializePaths()) {
            return;
        }
        isWindow2Visible[0] = !isWindow2Visible[0];
    }
    else if (numButton == 2) {
        if (!m_DataPath.initializePaths()) {
            return;
        }

        getMapOpen getMapOpen_;
        std::wstring fileName = getMapOpen_.getMapOpen1(m_DataPath.getMapsPath());
        if (fileName == L"Error") return;

        std::wstring str2 = m_DataPath.getSavePath();
        DataMaps dataMaps_;
        dataMaps_.NameMaps(fileName);

        sf::Vector2f newPosition(0, 0);
        std::wstring wstr(m_DataPath.getSavePath());
        std::wstring wstr2(dataMaps_.getNameMaps());
        NewDirectory app(m_Window, m_Font, newPosition, wstr2, wstr, isExetTree, m_OptionsData);
        app.newDirectory();

        LoadComands LoadComands_(m_DataPath);
        m_Comands = LoadComands_.loadComands();

        m_Window.clear(sf::Color(0, 255, 0));

        m_Window.display();

        initializeButtonsComands();
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

void OwnerWindow::initializeButtonsComands() {

    m_Window.clear(sf::Color(0, 255, 0));

    m_Window.display();

    LoadComands LoadComands_(m_DataPath);
    m_Comands = LoadComands_.loadComands();
    //if (!m_Comands.load)
    //    return;
    const std::vector<std::string> buttonTextures = { "dataAvLoad\\img\\menu.png", "dataAvLoad\\img\\return.png", "dataAvLoad\\img\\save.png", "dataAvLoad\\img\\craft.png", "dataAvLoad\\img\\cam.png" };
    //const std::vector<std::string> buttonTextures = {  };

    int minus = 0;
    const int maxButton = static_cast<int>(m_Buttons.size());
    for (int i = 0; i < maxButton; ++i) {
        m_Buttons[i].shape.setSize(sf::Vector2f(static_cast<float>(m_ConstSize.x - 1), static_cast<float>(m_ConstSize.x - 1)));

        bool isVisibleButton = true;
        switch (i+1)
        {
        case 1:
            isVisibleButton = m_Comands.returnCmd.isVisibleButton;
            break;
        case 2:
            isVisibleButton = m_Comands.saveCmd.isVisibleButton;
            break;
        case 3:
            isVisibleButton = m_Comands.craftCmd.isVisibleButton;
            break;
        case 4:
            isVisibleButton = m_Comands.camCmd.isVisibleButton;
            break;
        case 5:
            isVisibleButton = m_Comands.statsCmd.isVisibleButton;
            break;
        case 6:
            isVisibleButton = m_Comands.clearCmd.isVisibleButton;
            break;
        case 7:
            isVisibleButton = m_Comands.cleanCmd.isVisibleButton;
            break;
        case 8:
            isVisibleButton = m_Comands.strCmd.isVisibleButton;
            break;
        case 9:
            isVisibleButton = m_Comands.agiCmd.isVisibleButton;
            break;
        case 10:
            isVisibleButton = m_Comands.intCmd.isVisibleButton;
            break;
        default:
            break;
        }

        //if (!isVisibleButton)
            //m_Buttons[i].isVisibleButton = isVisibleButton;
            m_Buttons[i].isVisibleButton = true;

        m_Buttons[i].shape.setPosition(sf::Vector2f(2, static_cast<float>((i - minus) * m_ConstSize.x + 2) - 100.0f));  // Размещаем квадраты по вертикали

        m_Buttons[i].shape.setOutlineColor(sf::Color::Black);
        m_Buttons[i].shape.setOutlineThickness(2);

        // Load texture
        if (!m_Buttons[i].texture.loadFromFile(buttonTextures[i])) {
            // Handle error
        }

        // Set texture to sprite
        m_Buttons[i].sprite.setTexture(m_Buttons[i].texture);
        m_Buttons[i].sprite.setPosition(m_Buttons[i].shape.getPosition() + sf::Vector2f(2, 2));
        m_Buttons[i].sprite.setScale(.12f, .12f);

    }
}


void OwnerWindow::initialize() {

    setupWindow();

    initializeButtonsComands();
    
    const std::vector<std::string> buttonMenuTextures = { "dataAvLoad\\img\\upload.png", "dataAvLoad\\img\\upLoadAll.png", "dataAvLoad\\img\\options.png", "dataAvLoad\\img\\exit.png"};
    //const std::vector<std::string> buttonTextures = {  };
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
    m_Window.create(sf::VideoMode(xW, yW), "AVLoad", sf::Style::None);

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

    //m_Window.setFramerateLimit(60);
}

void OwnerWindow::activeGameTrue(HWND& hWndWindow) {
    m_IsVisibleOwnerWindow = true;

    m_Window.setVerticalSyncEnabled(true);
    //m_Window.setFramerateLimit(60);


    RECT rect;
    if (GetClientRect(hWndWindow, &rect)) {
        const int width = rect.right - rect.left;
        const int height = rect.bottom - rect.top;
        const float x = width - 30.0f - 2.0f;
        const float y = (height / 4.0f) * 3.0f - 60.0f;
        //const float x = width - 350.0f - 2.0f;
        //const float y = height - 10.0f - ((m_Buttons.size()-1) * 20);


        shapeIsLoad.setPosition(sf::Vector2f(width/2 - shapeIsLoad.getSize().x/2, height / 2 - shapeIsLoad.getSize().y / 2));
        textureIsLoad.setPosition(shapeIsLoad.getPosition() + sf::Vector2f(8,3));

        sf::Vector2f newPosition = sf::Vector2f(x, y);
        updatePosition(newPosition);
    }
}

void OwnerWindow::activeGameFalse(HWND& hWndWindow) {
    m_IsVisibleOwnerWindow = false;

    m_Window.setVerticalSyncEnabled(false);
    m_Window.setFramerateLimit(10);
}


void OwnerWindow::updatePosition(const sf::Vector2f& newPoition)
{
    unsigned i = 0;
    for (Button& buttonMenu : m_ButtonsMenu) {

        buttonMenu.shape.setPosition(sf::Vector2f(2, static_cast<float>(i * m_ConstSize.x + 2)-100.0f) + newPoition);
        buttonMenu.sprite.setPosition(buttonMenu.shape.getPosition() + sf::Vector2f(2, 2));

        i++;
    }

    //i = 0;
    int minus = 0;
    for (Button& button : m_Buttons) {
        if (!button.isVisibleButton)
            minus++;
        button.shape.setPosition(sf::Vector2f(2, static_cast<float>((i - minus) * m_ConstSize.x + 2) - 100.0f) + newPoition);
        button.sprite.setPosition(button.shape.getPosition() + sf::Vector2f(2, 2));

        i++;
    }
}

void OwnerWindow::setIsVisibleMenu(const bool& t_IsVisibleMenu) { m_IsVisibleMenu = t_IsVisibleMenu; }

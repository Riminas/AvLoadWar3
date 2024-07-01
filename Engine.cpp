#include <wtypes.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <TlHelp32.h>
#include <thread>
#include <chrono>
#include <windows.h>
#include <unordered_map>
#include "Engine.h"
#include "LoadManager.h"
#include "SkillsUpgradeStart.h"
#include "EngineFileTip1.h"
#include "key.h"

bool isExetTree = false;

// Глобальные переменные для хуков
HHOOK g_hHook = NULL;
std::unordered_map<int, std::chrono::high_resolution_clock::time_point> keyPressTimes;

Engine::Engine()
    : m_EngineFileTip2(m_Window, m_Font, m_DataPath, m_DataMaps, m_OptionsData), m_OwnerWindow(m_Window, m_Font, m_DataPath, isExetTree, m_IsVisibleLoad, m_OptionsData) // Инициализация EngineFileTip2 с ссылками на DataPath и DataMaps
{
}

NOTIFYICONDATA nid;

void Engine::engine1()
{
    if (!initialize())
        return;


    while (m_Window.isOpen()) {

        // Обработка сообщений Windows
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }


        processEvents();
        updateWindowVisibility();
        draw();

        if (isExetTree)
            m_Window.close();
    }

    // Удаляем иконку из трея при закрытии
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

bool Engine::IsWindowInFocus(HWND hWnd) {
    if (hWnd == NULL) {
        return 0;
    }
    wchar_t windowTitle[256];

    // Получаем имя окна
    int length = GetWindowText(hWnd, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
    if (length == 0) {
        return 0;
    }

    // Сравниваем имя окна с ожидаемым
    if (wcscmp(windowTitle, L"Warcraft III") != 0) {
        return 0;
    }

    return true;
}

void Engine::updateWindowVisibility()
{
    // Получаем дескриптор активного окна
    HWND hWndWindow = GetForegroundWindow();

    if (IsWindowInFocus(hWndWindow)) {
        m_OwnerWindow.activeGameTrue(hWndWindow);
        m_EngineFileTip2.activeGameTrue(hWndWindow);

        m_hWndWindow = hWndWindow;
        m_IsVisibleOwnerWindow = true;

        // Проверка состояния клавиш при каждом сообщении
        //if(m_IsWar126)
        if(m_OptionsData.autoСlickerKey || m_OptionsData.autoСlickerMaus)
            checkKeyStates();
    }
    else {
        m_OwnerWindow.activeGameFalse(hWndWindow);
        m_EngineFileTip2.activeGameFalse(hWndWindow);

        m_IsVisibleOwnerWindow = false;
    }
}

void Engine::processEvents()
{
    sf::Event event;
    const sf::Time time_delay = sf::milliseconds(50);

    while (m_Window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_Window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed && m_Clock.getElapsedTime() >= time_delay) {
            handleMouseButtonPressed(event);
        }
    }
}

void Engine::draw() {
    m_Window.clear(sf::Color(0, 255, 0));

    m_OwnerWindow.draw();

    if (m_IsVisibleOwnerWindow) {

        if (m_IsActiveWindow[0])
            m_EngineFileTip2.draw();
    }

    m_Window.display();
}

void Engine::handleMouseButtonPressed(sf::Event& event)
{
    bool lastIsActiveWindow[1]{ m_IsActiveWindow[0] };
    m_OwnerWindow.processingButtonMenu({ event.mouseButton.x, event.mouseButton.y }, lastIsActiveWindow);
    m_OwnerWindow.processingButton({ event.mouseButton.x, event.mouseButton.y }, lastIsActiveWindow);

    isInitialize(lastIsActiveWindow);

    if (m_IsActiveWindow[0]) {
        int i = m_EngineFileTip2.mouseButtonPressed(event, m_IsActiveWindow[0]);
        if (i >= 0) {
            m_IsActiveWindow[0] = false;

            m_OwnerWindow.setIsVisibleMenu(false);

            m_IsVisibleLoad = true;


            draw();

            const std::wstring wstr = m_EngineFileTip2.getPathListHero(i);

            LoadManager LoadManager_;
            LoadManager_.executeLoad(wstr);

            std::wstring nameMaps = m_DataMaps.getNameMaps();
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

            m_IsVisibleLoad = false;
        }
        else if (i == -2) {
            m_IsActiveWindow[0] = !m_IsActiveWindow[0];
        }
    }
    m_Clock.restart();
}

void Engine::isInitialize(bool lastIsActiveWindow[]) {
    if (lastIsActiveWindow[0] != m_IsActiveWindow[0]) {
        m_IsActiveWindow[0] = lastIsActiveWindow[0];
        if (m_EngineFileTip2.initialize() == 0) {
            m_IsActiveWindow[0] = false;
            m_OwnerWindow.setIsVisibleMenu(false);
        }
    }
}

bool Engine::initialize() {
    if (!m_Font.loadFromFile("dataAvLoad\\ArialUnicodeMS.ttf")) {
        return false;
    }

    m_OwnerWindow.initialize();

    initializeTree();

    return true;
}

void Engine::checkKeyStates() {
    auto now = std::chrono::high_resolution_clock::now();

    if (m_OptionsData.autoСlickerKey) {
        // Check keys 0-9 (0x30-0x39), A-Z (0x41-0x5A), num0-num9 (0x60-0x69), and right mouse button (VK_RBUTTON)
        for (int keyNum = 0x30; keyNum <= 0x69; ++keyNum) {
            if ((keyNum > 0x39 && keyNum < 0x41) || (keyNum > 0x5A && keyNum < 0x60) || keyNum > 0x69) {
                continue; // Skip non-relevant ranges
            }

            if (GetAsyncKeyState(keyNum) & 0x8000) { // Key is pressed
                if (keyPressTimes.find(keyNum) == keyPressTimes.end()) {
                    keyPressTimes[keyNum] = now;
                }
                else {
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - keyPressTimes[keyNum]).count();
                    if (elapsed >= 200) {
                        key key_;
                        while (GetAsyncKeyState(keyNum) & 0x8000) {
                            key_.key1(keyNum);
                            std::this_thread::sleep_for(std::chrono::milliseconds(5));
                        }
                        return;
                    }
                }
            }
            else {
                keyPressTimes.erase(keyNum);
            }
        }
    }

    if (m_OptionsData.autoСlickerMaus) {
        // Проверка правой кнопки мыши (VK_RBUTTON)
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) { // Правая кнопка мыши нажата
            if (keyPressTimes.find(VK_RBUTTON) == keyPressTimes.end()) {
                keyPressTimes[VK_RBUTTON] = now;
            }
            else {
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - keyPressTimes[VK_RBUTTON]).count();
                if (elapsed >= 200) {
                    key key_;
                    while (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
                        key_.keyVK(VK_RBUTTON, true);
                        std::this_thread::sleep_for(std::chrono::milliseconds(60));
                    }
                    return;
                }
            }
        }
        else {
            if (keyPressTimes.find(VK_RBUTTON) != keyPressTimes.end()) {
                key key_;
                key_.keyVK(VK_RBUTTON, false); // Отпускание правой кнопки мыши
                keyPressTimes.erase(VK_RBUTTON);
            }
        }
    }
}


// Константы для иконки в трее
#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_APP_ICON 5000
#define ID_TRAY_EXIT 3000

// Глобальные переменные
HWND hWnd;

// Обработчик событий окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_TRAYICON:
        if (lParam == WM_RBUTTONUP) {
            // Создаем контекстное меню
            HMENU hMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, TEXT("Exet"));

            // Показываем меню
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);
            TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y + 35, 0, hwnd, NULL);
            PostMessage(hwnd, WM_NULL, 0, 0); // Эта строка предотвращает возможные проблемы с меню
            DestroyMenu(hMenu);
        }
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_TRAY_EXIT) {
            // Закрываем приложение
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            isExetTree = true;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        isExetTree = true;
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void Engine::initializeTree() {

    // Получаем HWND окна
    hWnd = m_Window.getSystemHandle();
    std::wstring wstr = L"AVLoad_Tree";
    // Регистрируем класс окна для обработки событий
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, wstr.c_str(), NULL };
    RegisterClassEx(&wc);
    // Создаем невидимое окно для обработки событий
    hWnd = CreateWindow(wstr.c_str(), L"AVLoad Tree", WS_OVERLAPPEDWINDOW, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wc.hInstance, NULL);

    // Сворачиваем окно в трей
    MinimizeToTray();
}

// Функция для загрузки иконки из файла
HICON LoadIconFromFile(const wchar_t* filename) {
    return (HICON)LoadImage(NULL, filename, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
}

// Функция для сворачивания окна в трей
void Engine::MinimizeToTray() {
    // Создаем иконку в трее
    ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = ID_TRAY_APP_ICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    //nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    nid.hIcon = LoadIconFromFile(L"dataAvLoad\\img\\icon.ico");
    wcscpy_s(nid.szTip, L"AVLoad tree");
    Shell_NotifyIcon(NIM_ADD, &nid);
}

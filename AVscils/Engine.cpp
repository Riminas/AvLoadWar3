#include <wtypes.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <TlHelp32.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <windows.h>
#include <unordered_map>
#include "Engine.h"
#include "LoadManager.h"
#include "SkillsUpgradeStart.h"
#include "EngineFileTip1.h"
#include "key.h"
#include "LoadCommands.h"
#include "StringConvector.h"

bool isExetTree = false;

// ���������� ���������� ��� �����
HHOOK g_hHook = NULL;
std::unordered_map<int, std::chrono::high_resolution_clock::time_point> keyPressTimes;

Engine::Engine()
    : m_EngineFileTip2(m_Window, m_Font, m_DataAll), m_OwnerWindow(m_Window, m_Font, m_DataAll, isExetTree, m_IsVisibleLoad) // ������������� EngineFileTip2 � �������� �� DataPath � DataMaps
{
}

NOTIFYICONDATA nid;

void Engine::engine1()
{
    if (!initialize())
        return;


    while (m_Window.isOpen()) {

        // ��������� ��������� Windows
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

    // ������� ������ �� ���� ��� ��������
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

bool Engine::IsWindowInFocus(HWND hWnd) {
    if (hWnd == NULL) {
        return 0;
    }
    wchar_t windowTitle[256];

    // �������� ��� ����
    int length = GetWindowText(hWnd, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
    if (length == 0) {
        return 0;
    }

    // ���������� ��� ���� � ���������
    if (wcscmp(windowTitle, L"Warcraft III") != 0) {
        return 0;
    }

    return true;
}

void Engine::updateWindowVisibility()
{
    // �������� ���������� ��������� ����
    HWND hWndWindow = GetForegroundWindow();

    if (IsWindowInFocus(hWndWindow)) {
        if (m_NewIsVisible == true)
            return;

        if (m_IsVisibleOwnerWindow == false) {
            NewDataAll NewDataAll_(m_DataAll, m_Window, m_Font);
            NewDataAll_.newWarcrft(hWndWindow);
            if (m_DataAll.isNewWarcrft) {
                m_Window.setVisible(true);
                m_OwnerWindow.initializeButtonsCommands();
            }
            else {
                m_NewIsVisible = true;
                m_Window.setVisible(false);
                return;
            }
        }

        m_OwnerWindow.activeGameTrue(hWndWindow);

        if (m_DataAll.m_DataMaps.m_IsNewInfo) {
            m_IsActiveWindow[0] = false;
            m_DataAll.m_DataMaps.m_IsNewInfo = false;
            m_DataAll.m_DataMaps.m_PutSaveCode = m_DataAll.m_DataMaps.m_LastPathSaveCode;
            m_EngineFileTip2.initialize();
        }

        m_EngineFileTip2.activeGameTrue(hWndWindow);
        
        m_IsVisibleOwnerWindow = true;

        // �������� ��������� ������ ��� ������ ���������
        if(m_DataAll.versionWarcraft3 == 2)
            if(m_DataAll.m_OptionsData.auto�lickerKey || m_DataAll.m_OptionsData.auto�lickerMaus)
                checkKeyStates();
    }
    else {
        m_NewIsVisible = false;
        m_OwnerWindow.activeGameFalse();

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
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && m_Clock.getElapsedTime() >= time_delay) {
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

            LoadManager LoadManager_(m_DataAll.m_DataPath.m_hWndWindow);
            LoadManager_.executeLoad(wstr);

            std::wstring nameMaps = m_DataAll.m_DataMaps.getNameMaps();
            SkillsUpgradeStart ScilsUpgradeStart_(nameMaps);
            ScilsUpgradeStart_.skillsUpgradeStart();

            LoadCommands LoadCommands_(m_DataAll);
            LoadCommands_.loadCommands();


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

    if (m_DataAll.m_OptionsData.auto�lickerKey) {
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

    if (m_DataAll.m_OptionsData.auto�lickerMaus) {
        // �������� ������ ������ ���� (VK_RBUTTON)
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) { // ������ ������ ���� ������
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
                key_.keyVK(VK_RBUTTON, false); // ���������� ������ ������ ����
                keyPressTimes.erase(VK_RBUTTON);
            }
        }
    }
}


// ��������� ��� ������ � ����
#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_APP_ICON 5000
#define ID_TRAY_EXIT 3000
#define ID_TRAY_WARCRAFT 4000
#define ID_WARCRAFT_MENU_START 5000

// ���������� ����������
HWND hWnd;
std::map<std::wstring, std::wstring> buttonPaths; // ��� �������� ���� ������ � ����� ��������

bool LoadButtonData(const wchar_t* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE || fileSize == 0) {
        CloseHandle(hFile);
        return false;
    }

    char* buffer = new char[fileSize + 1];
    DWORD bytesRead;
    BOOL result = ReadFile(hFile, buffer, fileSize, &bytesRead, NULL);
    CloseHandle(hFile);

    if (!result || bytesRead == 0) {
        delete[] buffer;
        return false;
    }

    buffer[bytesRead] = '\0'; // ��������� null-����������

    std::string content(buffer);
    delete[] buffer;

    StringConvector StringConvector_;
    std::wstring wcontent = StringConvector_.utf8_to_utf16(content);

    if (wcontent[wcontent.size() - 1] != L'\n')
        wcontent = wcontent + L'\n';

    size_t pos = 0;
    while ((pos = wcontent.find(L'\n')) != std::wstring::npos) {
        std::wstring line = wcontent.substr(0, pos);
        wcontent.erase(0, pos + 1);

        size_t bracketPos = line.find(L']');
        if (bracketPos != std::wstring::npos) {
            std::wstring buttonName = line.substr(1, bracketPos - 1);
            std::wstring programPath = line.substr(bracketPos + 2); // ���������� "] "
            if (programPath[programPath.size() - 1] == L'\r')
                programPath.erase(programPath.size() - 1);

            // ���������, ���������� �� programPath �� ����� A-Z ��� a-z
            if (!programPath.empty() && !iswalpha(programPath[0])) {
                programPath.erase(0, 1); // ������� ������ ������
            }

            buttonPaths[buttonName] = programPath;
        }
    }

    return !buttonPaths.empty();
}

//// ������� �������� ������ �� ����� Path.ini
//bool LoadButtonData(const wchar_t* filename) {
//    std::wifstream file(filename);
//    std::wstring line;
//    bool dataLoaded = false;
//
//    while (std::getline(file, line)) {
//        size_t pos = line.find(L']');
//        if (pos != std::wstring::npos) {
//            std::wstring buttonName = line.substr(1, pos - 1);
//            std::wstring programPath = line.substr(pos + 2); // ���������� "] "
//            buttonPaths[buttonName] = programPath;
//            dataLoaded = true;
//        }
//    }
//
//    return dataLoaded;
//}

// ������� ������� ��������� � ������� ��������������
void LaunchProgramWithAdminRights(LPCWSTR programPath) {
    SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
    sei.lpVerb = L"runas";
    sei.lpFile = programPath;
    sei.nShow = SW_SHOWNORMAL;

    if (!ShellExecuteEx(&sei)) {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED) {
            MessageBox(NULL, L"������ ��������� ��� ������� �������������.", L"����������", MB_OK | MB_ICONINFORMATION);
        }
    }
}

// ���������� ������� ����
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_TRAYICON:
        if (lParam == WM_RBUTTONUP) {
            // ������� ����������� ����
            HMENU hMenu = CreatePopupMenu();

            // ���������, ��������� �� ������ �� StartPath.ini
            if (!buttonPaths.empty()) {
                HMENU hWarcraftMenu = CreatePopupMenu();
                // ��������� �������� � ������� Warcraft
                int buttonId = ID_WARCRAFT_MENU_START;
                for (const auto& [buttonName, programPath] : buttonPaths) {
                    AppendMenu(hWarcraftMenu, MF_STRING, buttonId++, buttonName.c_str());
                }
                AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hWarcraftMenu, TEXT("Warcraft"));
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL); // ��������� �������������� �����
            }

            AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, TEXT("Exit"));

            // ���������� ����
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);
            TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y + 35, 0, hwnd, NULL);
            PostMessage(hwnd, WM_NULL, 0, 0); // ��� ������ ������������� ��������� �������� � ����
            DestroyMenu(hMenu);
        }
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_TRAY_EXIT) {
            // ��������� ����������
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            isExetTree = true;
        }
        else if (LOWORD(wParam) >= ID_WARCRAFT_MENU_START) {
            int buttonIndex = LOWORD(wParam) - ID_WARCRAFT_MENU_START;
            if (buttonIndex >= 0 && buttonIndex < buttonPaths.size()) {

                auto it = std::next(buttonPaths.begin(), buttonIndex);
                LaunchProgramWithAdminRights(it->second.c_str());
            }
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
    // �������� HWND ����
    hWnd = m_Window.getSystemHandle();
    std::wstring wstr = L"AVLoad_Tree";
    // ������������ ����� ���� ��� ��������� �������
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, wstr.c_str(), NULL };
    RegisterClassEx(&wc);
    // ������� ��������� ���� ��� ��������� �������
    hWnd = CreateWindow(wstr.c_str(), L"AVLoad Tree", WS_OVERLAPPEDWINDOW, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wc.hInstance, NULL);

    // �������� ������ �� ����� StartPath.ini
    if (!LoadButtonData(L"dataAvLoad\\StartPath.ini")) {
        buttonPaths.clear(); // ������� ������, ���� ���� ���� ��� �� ����������
    }

    // ����������� ���� � ����
    MinimizeToTray();
}

// ������� ��� �������� ������ �� �����
HICON LoadIconFromFile(const wchar_t* filename) {
    return (HICON)LoadImage(NULL, filename, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
}

// ������� ��� ������������ ���� � ����
void Engine::MinimizeToTray() {
    // ������� ������ � ����
    ZeroMemory(&nid, sizeof(NOTIFYICONDATA));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = ID_TRAY_APP_ICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIconFromFile(L"dataAvLoad\\img\\icon.ico");
    wcscpy_s(nid.szTip, L"AVLoad tree");
    Shell_NotifyIcon(NIM_ADD, &nid);
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <locale>
#include <stdexcept>
#include <windows.h>
#include <SFML/Graphics.hpp>
#include <wtypes.h>
#include <TlHelp32.h>

#include "OpenGuide.h"
#include "StringToString.h"

// Основной цикл обработки сообщений
void OpenGuide::messageLoop(sf::RenderWindow& window2, std::vector<sf::Text>& textList, const sf::Font& font, int& startIndex, const unsigned& maxCount, const size_t& guideSize) {
    window2.clear();
    for (const sf::Text& text : textList) {
        window2.draw(text);
    }
    window2.display();

    while (window2.isOpen()) {

        bool escPressed = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
        if (escPressed) {
            window2.close();
            return;
        }
        bool wPressed = GetAsyncKeyState(87) & 0x8000;
        bool sPressed = GetAsyncKeyState(83) & 0x8000;

        if (!(wPressed && sPressed)) {
            if (wPressed) {
                keyDownW = true;
                keyDownS = false;
            }
            else {
                keyDownW = false;
            }

            if (sPressed) {
                keyDownS = true;
                keyDownW = false;
            }
            else {
                keyDownS = false;
            }
        }
        const int in = startIndex + maxCount + 1;
        if (keyDownS &&  in < guideSize) {
            updateTextListNew(textList, font, ++startIndex, maxCount, true);
            saveStartIndex(startIndex);

            window2.clear();
            for (const sf::Text& text : textList) {
                window2.draw(text);
            }
            window2.display();
        }
        else if (keyDownW && startIndex - 1 > 0) {
            updateTextListNew(textList, font, --startIndex, maxCount, false);
            saveStartIndex(startIndex);

            window2.clear();
            for (const sf::Text& text : textList) {
                window2.draw(text);
            }
            window2.display();
        }
        Sleep(10);
    }
}

// Открытие гайда
void OpenGuide::openGuide(const std::string& fileName) {
    StringToString stringConverter;
    m_FileName = stringConverter.strVectorToStr(fileName, false);
    std::wstring path(m_FileName.begin(), m_FileName.end());

    if (!loadGuide(path)) {
        std::cout << "Error: open file (" << fileName << ")" << std::endl;
        return;
    }

    size_t guideSize = m_ListGuide.size();
    if (guideSize == 0)
        return;

    unsigned maxDisplayCount = 40;
    if (maxDisplayCount >= guideSize)
        maxDisplayCount = guideSize;

    int startIndex = 0;
    loadStartIndex(startIndex);

    sf::Font font;
    if (!font.loadFromFile("dataAvLoad\\ArialUnicodeMS.ttf")) {
        std::cout << "Error loading font\n";
        return;
    }

    std::vector<sf::Text> textList;
    initializeTextList(textList, font, maxDisplayCount);
    updateTextList(textList, startIndex, maxDisplayCount);

    sf::RenderWindow window2(sf::VideoMode(sf::VideoMode::getDesktopMode().width-31, sf::VideoMode::getDesktopMode().height+1), "AVLoad", sf::Style::None);
    window2.setPosition({ 0, 0 });
    setupWindow(window2);

    messageLoop(window2, textList, font, startIndex, maxDisplayCount, guideSize);
}

// Преобразование строки из UTF-8 в wstring
std::wstring OpenGuide::utf8ToWstring(const std::string& str) {
    std::wstring wstr;
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    if (len > 0) {
        wstr.resize(len);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], len);
    }
    return wstr;
}

// Загрузка гайда
bool OpenGuide::loadGuide(const std::wstring& path) {
    std::filesystem::path fullPath = std::filesystem::path(m_PathAllGuide + L"\\" + path);
    if (!std::filesystem::is_directory(fullPath))
        return false;

    fullPath = std::filesystem::path(m_PathAllGuide + L"\\" + path + m_NameGuide);

    std::ifstream file(fullPath);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        m_ListGuide.push_back(utf8ToWstring(line));
    }
    return true;
}

// Инициализация текстового списка
void OpenGuide::initializeTextList(std::vector<sf::Text>& textList, const sf::Font& font, int maxCount) {
    for (int i = 0; i < maxCount; i++) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        const float y = 30 + i * 18;
        text.setPosition(20, y);
        textList.push_back(text);
    }
}

// Обновление текста в списке
void OpenGuide::updateTextListNew(std::vector<sf::Text>& textList, const sf::Font& font, int startIndex, unsigned maxCount, bool isNext) {
    if (isNext) {
        textList.erase(textList.begin()); // Удаляем первый элемент
        
        int i = startIndex + maxCount + 1;
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        text.setPosition(sf::Vectorf(20, 30 + i * 18));
        text.setString(m_ListGuide[i]);
        textList.push_back(text);
    }
    else {
        textList.pop_back(); // Удаляем последний элемент

        int i = startIndex - 1;
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        text.setPosition(20, 30 + i * 18);
        text.setString(m_ListGuide[i]); // Добавляем первый элемент
        textList.insert(textList.begin(), text);
    }
    for (unsigned i = 0; i < maxCount; i++) {
        textList[i].setPosition(20, 30 + i * 18);
    }
}

// Обновление текста в списке
void OpenGuide::updateTextList(std::vector<sf::Text>& textList, int& startIndex, const unsigned& maxCount) {
    for (unsigned i = 0; i < maxCount; i++) {
        textList[i].setString(m_ListGuide[i + startIndex]);
    }
}

// Проверка наличия процесса Warcraft III
bool OpenGuide::isProcessWarcraftIII() {
    const std::wstring processName = L"Warcraft III.exe";
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 processEntry{};
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapshot, &processEntry)) {
        CloseHandle(snapshot);
        return false;
    }

    do {
        if (processEntry.szExeFile == processName) {
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &processEntry));

    CloseHandle(snapshot);
    return false;
}

// Настройка окна
void OpenGuide::setupWindow(sf::RenderWindow& window2) {
    HWND hwnd = window2.getSystemHandle();
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
    SetLayeredWindowAttributes(hwnd, 0, 220, LWA_ALPHA);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    window2.setFramerateLimit(30);
}

bool OpenGuide::saveStartIndex(const int& startIndex) {
    std::wstring path(m_FileName.begin(), m_FileName.end());
    std::filesystem::path fullPath = std::filesystem::path(m_PathAllGuide + L"\\" + path + m_NameData);

    std::ofstream fil(fullPath);
    fil << startIndex;
    fil.close();

    return true;
}

bool OpenGuide::loadStartIndex(int& startIndex) {
    std::wstring path(m_FileName.begin(), m_FileName.end());
    std::filesystem::path fullPath = std::filesystem::path(m_PathAllGuide + L"\\" + path + m_NameData);

    std::ifstream fil(fullPath);
    if (!fil.is_open())
        return false;
    fil >> startIndex;
    fil.close();

    return true;
}
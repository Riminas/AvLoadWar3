#include <sstream>
#include <Windows.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <TlHelp32.h>
#include "getMapOpen.h"
#include "EngineFile.h"
#include "load.h"
#include "NewPathSaveCode.h"

void EngineFile::engineFile1(bool isLastSaveCode)
{
    getMapOpen getMapOpen_;
    m_FileName = getMapOpen_.getMapOpen1(m_DataPatc.getMaps());
    if (m_FileName == "Error") return;

    std::string str = m_DataPatc.getSave();
    std::wstring str2(str.begin(), str.end());
    m_DataMaps.NameMaps(m_FileName);
    if(!m_DataMaps.PutSaveCode(str2)) return;

    bool isError = false;
    m_ListHero = m_DataPatc.getFileSaveCode1(m_DataMaps.getPutSaveCode(), isError);
    if (isError) return;

    for (const auto& p : m_ListHero) {
        if (p.nameChar.empty()) {
            isLastSaveCode = true;
            break;
        }
    }

    if (m_ListHero.size() == 1 || isLastSaveCode) {
        engineTip1(isLastSaveCode);
    }
    else {
        engineTip2();
    }
}

void EngineFile::engineTip1(const bool& isLastSaveCode)
{
    std::string patc;
    std::string nameChar;

    if (m_ListHero.size() > 1 && isLastSaveCode) {
        auto latestTime = std::filesystem::file_time_type();
        for (const auto& p : m_ListHero) {
            if (p.latestTime > latestTime) {
                latestTime = p.latestTime;
                patc = p.patc;
                nameChar = p.nameChar;
            }
        }
    }
    else {
        patc = m_ListHero.front().patc;
    }

    while (GetAsyncKeyState(VK_MENU) & 0x8000 || GetAsyncKeyState(0x30) & 0x8000) { // 'Alt + 0'
        Sleep(100);
    }
    std::cout << "Hero:  " << nameChar << std::endl;

    load load_;
    load_.load1(patc);
}

void EngineFile::engineTip2()
{
    sf::Font font;
    if (!font.loadFromFile("dataAvLoad\\arialmt.ttf")) {
        std::cout << "Error loading font\n";
        return;
    }

    std::vector<sf::Text> listHeroText;
    listHeroText.emplace_back("Alt + 0 cancel", font, 16);

    for (size_t i = 0; i < m_ListHero.size(); ++i) {
        std::string str = "\tAlt + " + std::to_string(i + 1) + " Prof( " + m_ListHero[i].nameChar + " )";
        listHeroText.emplace_back(str, font, 16);
        listHeroText.emplace_back(("\t\t" + file_time_to_string(m_ListHero[i].latestTime)), font, 14);
    }

    sf::RenderWindow window2(sf::VideoMode(350, sf::VideoMode::getDesktopMode().height), "AVLoad", sf::Style::None);
    window2.setPosition({ 0, 0 });
    SetWindowPos(window2.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    for (size_t i = 0; i < listHeroText.size(); ++i) {
        listHeroText[i].setPosition(30, 100 + 30 * i - (i % 2 * -10));
        window2.draw(listHeroText[i]);
    }

    window2.display();
    window2.setFramerateLimit(1);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        if (msg.message == WM_HOTKEY) {
            if (msg.wParam == 1) {
                window2.close();
                break;
            }
            else if (msg.wParam >= 2 && msg.wParam <= 10) {
                if (!isProcessWarcraftIII()) {
                    window2.close();
                    break;
                }

                while (GetAsyncKeyState(VK_MENU) & 0x8000 || GetAsyncKeyState(0x30 + msg.wParam - 1) & 0x8000) {
                    Sleep(100);
                }
                if (msg.wParam - 2 < m_ListHero.size()) {
                    window2.close();
                    std::cout << "Hero:  " << m_ListHero[msg.wParam - 2].nameChar << std::endl;
                    Sleep(100);
                    load load_;
                    load_.load1(m_ListHero[msg.wParam - 2].patc);
                    break;
                }
            }
        }
    }
}

bool EngineFile::isProcessWarcraftIII()
{
    const std::wstring nameProces_136 = L"Warcraft III.exe";
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe)) {
        CloseHandle(hSnapshot);
        return false;
    }

    do {
        if (pe.szExeFile == nameProces_136) {
            CloseHandle(hSnapshot);
            return true;
        }
    } while (Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
    return false;
}

std::string EngineFile::file_time_to_string(const std::filesystem::file_time_type& ftime)
{
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
    );

    std::time_t time = std::chrono::system_clock::to_time_t(sctp);
    std::tm tm;
    localtime_s(&tm, &time);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
    return oss.str();
}

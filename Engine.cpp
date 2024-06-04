#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "Engine.h"
#include "EngineFile.h"
#include <shtypes.h>
#include <ShlObj.h>
#include "getMapOpen.h"
#include "DataMaps.h"
#include "NewPathSaveCode.h"

void Engine::engine1()
{
    registerAllHotKey();

    while (true) {
        std::cout << getStartCout();

        isWhileProcessWarcraftIII();

        std::cout << getRazelCout();
        if (!m_DataPatc.loadDataOptions()) return;

        std::cout << getRazelCout();
        std::cout << getStartCout2();


        whileMSG();
    }
}

bool Engine::isProcessWarcraftIII()
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

    bool isRunning = false;
    do {
        if (pe.szExeFile == nameProces_136) {
            isRunning = true;
            break;
        }
    } while (Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
    return isRunning;
}

void Engine::isWhileProcessWarcraftIII() {
    while (!isProcessWarcraftIII()) {
        Sleep(CLOCKS_PER_SEC);
    }
}

void Engine::whileMSG() {
    const int time_delay = 200;
    clock_t now = clock();

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            if (msg.wParam == 1 || msg.wParam == 10) {
                if (clock() - now >= time_delay) {
                    std::cout << getRazelCout();
                    EngineFile EngineFile_(m_DataPatc);
                    EngineFile_.engineFile1(msg.wParam == 1);
                    now = clock();
                }
            }
            if (msg.wParam == 9) {

                std::string fileName;
                getMapOpen getMapOpen_;
                fileName = getMapOpen_.getMapOpen1(m_DataPatc.getMaps());
                if (fileName != "Error") {
                    DataMaps dataMaps;
                    std::string str = m_DataPatc.getSave();
                    std::wstring str2(str.begin(), str.end());
                    dataMaps.NameMaps(fileName);


                    str = m_DataPatc.getSave();
                    std::wstring wstr = { str.begin(), str.end() };
                    NewPathSaveCode NewPathSaveCode_(wstr, dataMaps.getNameMaps());
                    NewPathSaveCode_.newPathSaveCode();
                }
            }
        }
    }
}

void Engine::registerAllHotKey()
{
    for (int i = 0; i <= 9; ++i) {
        RegisterHotKey(NULL, i + 1, MOD_ALT | MOD_NOREPEAT, 48 + i);
    }
}

LPITEMIDLIST GetPIDLFromPath(const std::wstring& path) {
    LPITEMIDLIST pidl = nullptr;
    HRESULT hr = SHParseDisplayName(path.c_str(), nullptr, &pidl, 0, nullptr);
    return SUCCEEDED(hr) ? pidl : nullptr;
}

std::string Engine::getStartCout() { return "\tThe game is expected to launch\nhttps://github.com/Riminas/AvLoadWar3.git\n\n"; }
std::string Engine::getStartCout2() { return "\tTo download the last save, press the key combination ' alt + 0 '\n\tLoading the list of heroes ' alt + 9 '\n\tRemove path code ' alt + 8 '\n"; }
std::string Engine::getRazelCout() { return "___________________________________________________________________________________\n"; }

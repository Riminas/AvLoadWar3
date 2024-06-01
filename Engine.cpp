#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "Engine.h"
#include "EngineFile.h"

void Engine::engine1()
{
    registerAllHotKey();

    while (true) {
        std::cout << getStartCout();

        isWhileProcessWarcraftIII();
        
        std::cout << getRazelCout();
        std::cout << getStartCout2();

        whileMSG();
       
    }
}

bool Engine::isProcessWarcraftIII()// варкрафт запущен изи закрыт
{
    const std::wstring nameProces_136 = L"Warcraft III.exe";

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe)) {
        CloseHandle(hSnapshot);
        return 0;
    }

    do {
        if (pe.szExeFile == nameProces_136) {
            CloseHandle(hSnapshot);
            return 1;
        }
    } while (Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
    return 0;
}

void Engine::isWhileProcessWarcraftIII() {
    while (true) {
        if (isProcessWarcraftIII()) {
            break;
        }
        Sleep(500);
    }
}

void Engine::whileMSG() {
    int time_delay = 1;
    time_delay *= CLOCKS_PER_SEC;
    clock_t now = clock();

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            if (msg.wParam == 1 || msg.wParam == 10)
            {
                if (!(clock() - now < time_delay)) {// 1 загрузка в 1 секунду (анти спам и зависания)

                    std::cout << getRazelCout();
                    EngineFile EngineFile_;
                    if (msg.wParam == 1) EngineFile_.engineFile1(true);
                    else if(msg.wParam == 10) EngineFile_.engineFile1(false);
                    now = clock();
                }
            }
        }
    }
}

void Engine::registerAllHotKey()
{
    RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, 48);
    RegisterHotKey(NULL, 2, MOD_ALT | MOD_NOREPEAT, 49);
    RegisterHotKey(NULL, 3, MOD_ALT | MOD_NOREPEAT, 50);
    RegisterHotKey(NULL, 4, MOD_ALT | MOD_NOREPEAT, 51);
    RegisterHotKey(NULL, 5, MOD_ALT | MOD_NOREPEAT, 52);
    RegisterHotKey(NULL, 6, MOD_ALT | MOD_NOREPEAT, 53);
    RegisterHotKey(NULL, 7, MOD_ALT | MOD_NOREPEAT, 54);
    RegisterHotKey(NULL, 8, MOD_ALT | MOD_NOREPEAT, 55);
    RegisterHotKey(NULL, 9, MOD_ALT | MOD_NOREPEAT, 56);
    RegisterHotKey(NULL, 10, MOD_ALT | MOD_NOREPEAT, 57);
}

std::string Engine::getStartCout() { return "\tThe game is expected to launch\nhttps://github.com/Riminas/AvLoadWar3.git\n\n"; }
std::string Engine::getStartCout2() { return "\tTo download the last save, press the key combination ' alt + 0 '\n\tLoading the list of heroes ' alt + 9 '\n"; }
std::string Engine::getRazelCout() { return "___________________________________________________________________________________\n"; }
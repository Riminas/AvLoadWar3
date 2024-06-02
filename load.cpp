#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <future>
#include <sstream>

#include "load.h"
#include "key.h"

load::load() {
    m_targetWindow = FindWindow(NULL, L"Warcraft III");
}

bool IsEqualKL(HKL hKL, WORD wLanguage) {
    const BYTE* pb = (const BYTE*)hKL;
    return (pb[0] == 0) && (pb[2] == wLanguage);
}

bool load::load1(const std::string& put) {
    m_Put = put;
    key key_;

    std::vector<std::string> codeAll(10);

    if (codeAllProv(codeAll)) {
        SetForegroundWindow(m_targetWindow);
        Sleep(100);

        if (m_TipLoad == 1) {
            PostMessage(m_targetWindow, WM_INPUTLANGCHANGEREQUEST, 1, 0x04090409);
            Sleep(100);
            coutCommands("-load");
        }

        for (const auto& code : codeAll) {
            if (code.empty())
                break;

            Sleep(100);
            SetForegroundWindow(m_targetWindow);
            Sleep(500);

            while (GetAsyncKeyState(16) & 0x8000 || GetAsyncKeyState(17) & 0x8000 || GetAsyncKeyState(18) & 0x8000) {
                Sleep(100);
            }

            PostMessage(m_targetWindow, WM_INPUTLANGCHANGEREQUEST, 1, 0x04090409);
            Sleep(200);

            key_.key1(0x0D); // Enter to open chat
            convectorCharKey(code);
            key_.key1(0x0D); // Enter to close chat

            Sleep(10);
            std::cout << "codes: ( " << code << " )" << std::endl;
        }
    }
    Sleep(100);
    PostMessage(m_targetWindow, WM_INPUTLANGCHANGEREQUEST, 1, 0x4190419);

    return codeAllProv(codeAll);
}

bool load::codeAllProv(std::vector<std::string>& codeAll) {
    std::ifstream file(m_Put);
    if (!file.is_open())
        return false;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{});

        for (size_t i = 0; i < tokens.size(); ++i) {
            if (tokens[i].size() == 7) {
                if (tokens[i] == "\"Code1:") {
                    m_TipLoad = 1;
                    provOne(codeAll[0], tokens, i + 1);
                }
                else if (tokens[i] == "\"Code2:") {
                    provOne(codeAll[1], tokens, i + 1);
                }
            }
            else if (tokens[i].size() == 6 && tokens[i] == "\"-load") {
                provOne(codeAll[0], tokens, i + 1, "-load ");
                break;
            }
            else if (tokens[i] == "Preload(" && tokens[i + 1].size() >= 25 && tokens[i + 2] == ")") {
                provOne(codeAll[0], tokens, i + 1, "-load ");
                break;
            }
        }
    }

    return true;
}

void load::provOne(std::string& code, const std::vector<std::string>& tokens, size_t index, const std::string& prefix) {
    if (tokens.size() > index) {
        while (index < tokens.size() && tokens[index] == ":") {
            ++index;
        }

        code = tokens[index];
        if (code.back() == '"' && tokens[index + 1] == ")") {
            code.pop_back();
            if (code.front() == '"') {
                code.erase(0, 1);
            }
        }
        else if (code.size() > 1 && code.back() == ')' && code[tokens[index].size() - 2] == '"') {
            code.erase(tokens[index].size() - 2, 2);
        }

        if (!prefix.empty()) {
            code.insert(0, prefix);
        }
    }
}

void load::convectorCharKey(const std::string& code) {
    key key_;
    std::unordered_map<char, std::pair<int, bool>> keyMap = {
        {' ', {32, false}}, {'!', {49, true}}, {'"', {222, true}}, {'#', {51, true}},
        {'$', {52, true}}, {'%', {53, true}}, {'&', {55, true}}, {'\'', {222, false}},
        {'(', {57, true}}, {')', {48, true}}, {'*', {56, true}}, {'+', {187, true}},
        {',', {188, false}}, {'-', {189, false}}, {'.', {190, false}}, {'/', {191, false}},
        {':', {186, true}}, {';', {186, false}}, {'<', {188, true}}, {'=', {187, false}},
        {'>', {190, true}}, {'?', {191, true}}, {'@', {50, true}}, {'[', {219, false}},
        {'\\', {229, false}}, {']', {221, false}}, {'^', {54, true}}, {'_', {189, true}},
        {'`', {192, false}}, {'{', {219, true}}, {'|', {229, true}}, {'}', {221, true}},
        {'~', {192, true}}
    };

    for (char c : code) {
        if (c >= 'a' && c <= 'z') {
            key_.key1(c - 32); // Convert to uppercase
        }
        else if (c >= 'A' && c <= 'Z') {
            key_.key2(c, VK_SHIFT); // Press shift for uppercase letters
        }
        else if (c >= '0' && c <= '9') {
            key_.key1(c); // Numeric keys
        }
        else if (keyMap.find(c) != keyMap.end()) {
            auto [vk, shift] = keyMap[c];
            if (shift) {
                key_.key2(vk, VK_SHIFT);
            }
            else {
                key_.key1(vk);
            }
        }
        else {
            std::cerr << "ERROR char( " << c << " )" << std::endl;
        }
    }
}

void load::coutCommands(const std::string& strCommand) {
    std::cout << "Command: " << strCommand << std::endl;
    key key_;
    key_.key1(0x0D); // Enter to open chat
    convectorCharKey(strCommand);
    key_.key1(0x0D); // Enter to close chat
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "StringConvector.h"
#include "LoadDataFail.h"
#include "DataAll.h"
#include "LoadDataCommands.h"
#include "getMapOpen.h"

LoadDataCommands::LoadDataCommands(DataAll& t_DataAll)
    : m_DataAll(t_DataAll)
{
}

void LoadDataCommands::writeDefaultDataCommands(const std::wstring& fullPath, const bool isDataDefaul) {
    std::ofstream fil(fullPath.c_str());
    if (isDataDefaul) {
        for (const auto& cmd : m_DataAll.m_DataCommands.commandMapDefault) {
            fil << cmd.first << std::endl;
            fil << "cmd=" << cmd.second.str << std::endl;
            fil << "isVisibleButton=" << std::boolalpha << cmd.second.isVisibleButton << std::endl;
            fil << "isLoad=" << std::boolalpha << cmd.second.isLoad << std::endl;
            fil << "hotKey=" << cmd.second.hotKey << std::endl;
            fil << std::endl; // Пустая строка между командами для удобочитаемости
        }
    }
    else {
        for (const auto& cmd : m_DataAll.m_DataCommands.commandMap) {
            fil << cmd.first << std::endl;
            fil << "cmd=" << cmd.second->str << std::endl;
            fil << "isVisibleButton=" << std::boolalpha << cmd.second->isVisibleButton << std::endl;
            fil << "isLoad=" << std::boolalpha << cmd.second->isLoad << std::endl;
            fil << "hotKey=" << cmd.second->hotKey << std::endl;
            fil << std::endl; // Пустая строка между командами для удобочитаемости
        }
    }
    fil.close();
}

void LoadDataCommands::parseCommandLine(const std::wstring& fullLine) {
    static std::string currentCmd;
    std::wistringstream wiss(fullLine);
    std::wstring line;

    while (std::getline(wiss, line)) {
        // Удаление символов возврата каретки \r
        line.erase(std::remove(line.begin(), line.end(), L'\r'), line.end());

        StringConvector StringConvector_;
        const std::string strLine{ StringConvector_.utf16_to_utf8(line) };
        std::istringstream iss(strLine);
        std::string firstWord;

        if (iss >> firstWord) {
            if (firstWord[0] == '[' && firstWord.back() == ']') {
                // Это название команды
                currentCmd = firstWord;
            }
            //else if (firstWord.rfind("cmd=", 0) == 0) {
            //    // Строка команды
            //    std::string cmdStr = firstWord.substr(4); // Получаем все после "cmd="
            //    std::getline(iss, cmdStr); // Считываем оставшуюся часть строки, если есть
            //    if (!cmdStr.empty() && cmdStr[0] == ' ') {
            //        cmdStr.erase(0, 1);
            //    }
            //    if (m_DataAll.m_DataCommands.commandMap.find(currentCmd) != m_DataAll.m_DataCommands.commandMap.end()) {
            //        m_DataAll.m_DataCommands.commandMap[currentCmd]->str = cmdStr;
            //    }
            //}
            else if (firstWord.rfind("cmd=", 0) == 0) {
                // Строка команды
                std::string cmdStr = firstWord.substr(4); // Получаем все после "cmd="
                std::string remainingLine;
                if (std::getline(iss, remainingLine)) {
                    cmdStr += /*" " + */remainingLine; // Считываем оставшуюся часть строки
                }
                if (m_DataAll.m_DataCommands.commandMap.find(currentCmd) != m_DataAll.m_DataCommands.commandMap.end()) {
                    m_DataAll.m_DataCommands.commandMap[currentCmd]->str = cmdStr;
                }
            }
            else if (firstWord.rfind("isVisibleButton=", 0) == 0) {
                // Видимость
                std::string paramValue = firstWord.substr(16); // Получаем все после "isVisibleButton="
                bool isVisible;
                std::istringstream(paramValue) >> std::boolalpha >> isVisible;
                if (m_DataAll.m_DataCommands.commandMap.find(currentCmd) != m_DataAll.m_DataCommands.commandMap.end()) {
                    m_DataAll.m_DataCommands.commandMap[currentCmd]->isVisibleButton = isVisible;
                }
            }
            else if (firstWord.rfind("isLoad=", 0) == 0) {
                // Флаг загрузки
                std::string paramValue = firstWord.substr(7); // Получаем все после "isLoad="
                bool isLoad;
                std::istringstream(paramValue) >> std::boolalpha >> isLoad;
                if (m_DataAll.m_DataCommands.commandMap.find(currentCmd) != m_DataAll.m_DataCommands.commandMap.end()) {
                    m_DataAll.m_DataCommands.commandMap[currentCmd]->isLoad = isLoad;
                }
            }
            else if (firstWord.rfind("hotKey=", 0) == 0) {
                // Горячая клавиша
                std::string paramValue = firstWord.substr(7); // Получаем все после "hotKey="
                int hotKey = paramValue.empty() ? 0 : std::stoi(paramValue); // Проверка на пустую строку
                if (m_DataAll.m_DataCommands.commandMap.find(currentCmd) != m_DataAll.m_DataCommands.commandMap.end()) {
                    m_DataAll.m_DataCommands.commandMap[currentCmd]->hotKey = hotKey;
                }
            }
            else {
                m_IsShouldRewrite = true;
            }
        }
    }
}

bool LoadDataCommands::starDataCommands() {
    const std::wstring putSaveCode = m_DataAll.m_DataMaps.getNameMaps();
    m_FullPath = m_putDatMapPut + L"\\" + putSaveCode;
    std::filesystem::create_directory(m_FullPath);

    m_FullPath += m_DataCommandsPath;

    return true;
}

void LoadDataCommands::saveDataCommands() {
    if (!starDataCommands()) {
        m_DataAll.m_DataCommands.load = false;
        return;
    }

    writeDefaultDataCommands(m_FullPath, false);
    m_DataAll.m_DataCommands.load = true;
}

void LoadDataCommands::loadDataCommands() {
    if (!starDataCommands()) {
        m_DataAll.m_DataCommands.load = false;
    }

    load();
    m_DataAll.m_DataCommands.load = true;
}

void LoadDataCommands::load() {
    LoadDataFail LoadDataFail_;
    const std::wstring fullLine = LoadDataFail_.loadDataFail(m_FullPath);

    if (fullLine.empty()) {
        std::wcerr << L"Unable to open file for writing: " << m_FullPath << std::endl;
        writeDefaultDataCommands(m_FullPath, true);
        const std::wstring fullLine2 = LoadDataFail_.loadDataFail(m_FullPath);
        parseCommandLine(fullLine2);
        return;
    }

    parseCommandLine(fullLine);

    writeDefaultDataCommands(m_FullPath, false);
}


//void LoadDataCommands::load() {
//    std::ifstream file(m_FullPath.c_str());
//    if (!file.is_open()) {
//        std::wcerr << L"Unable to open file for writing: " << m_FullPath << std::endl;
//        writeDefaultDataCommands(m_FullPath);
//    }
//    else {
//        std::string line;
//        while (std::getline(file, line)) {
//            parseCommandLine(line);
//        }
//        file.close();
//    }
//}
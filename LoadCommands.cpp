#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "DataAll.h"
#include "LoadCommands.h"
#include "getMapOpen.h"

LoadCommands::LoadCommands(DataAll& t_DataAll)
    : m_DataAll(t_DataAll)
{
}

//void LoadCommands::writeDefaultCommands(const std::wstring& fullPath, Commands& Commands) {
//    std::ofstream fil(fullPath.c_str());
//    for (const auto& cmd : Commands.commandMap) {
//        fil << cmd.first << " " << cmd.second->str << std::endl;
//        fil << "[isVisibleButton" << cmd.first.substr(1) << " " << std::boolalpha << cmd.second->isVisibleButton << std::endl;
//        fil << "[isLoad" << cmd.first.substr(1) << " " << std::boolalpha << cmd.second->isLoad << std::endl;
//    }
//}

void LoadCommands::writeDefaultCommands(const std::wstring& fullPath) {
    std::ofstream fil(fullPath.c_str());
    fil << "[Return] " << m_DataAll.m_Commands.returnCmd.str << std::endl;
    fil << "[isVisibleButtonReturn] " << std::boolalpha << m_DataAll.m_Commands.returnCmd.isVisibleButton << std::endl;
    fil << "[isLoadReturn] " << std::boolalpha << m_DataAll.m_Commands.returnCmd.isLoad << std::endl;

    fil << "[Save] " << m_DataAll.m_Commands.saveCmd.str << std::endl;
    fil << "[isVisibleButtonSave] " << std::boolalpha << m_DataAll.m_Commands.saveCmd.isVisibleButton << std::endl;
    fil << "[isLoadSave] " << std::boolalpha << m_DataAll.m_Commands.saveCmd.isLoad << std::endl;

    fil << "[Craft] " << m_DataAll.m_Commands.craftCmd.str << std::endl;
    fil << "[isVisibleButtonCraft] " << std::boolalpha << m_DataAll.m_Commands.craftCmd.isVisibleButton << std::endl;
    fil << "[isLoadCraft] " << std::boolalpha << m_DataAll.m_Commands.craftCmd.isLoad << std::endl;

    fil << "[Cam] " << m_DataAll.m_Commands.camCmd.str << std::endl;
    fil << "[isVisibleButtonCam] " << std::boolalpha << m_DataAll.m_Commands.camCmd.isVisibleButton << std::endl;
    fil << "[isLoadCam] " << std::boolalpha << m_DataAll.m_Commands.camCmd.isLoad << std::endl;

    fil << "[Stats] " << m_DataAll.m_Commands.statsCmd.str << std::endl;
    fil << "[isVisibleButtonStats] " << std::boolalpha << m_DataAll.m_Commands.statsCmd.isVisibleButton << std::endl;
    fil << "[isLoadStats] " << std::boolalpha << m_DataAll.m_Commands.statsCmd.isLoad << std::endl;

    fil << "[Clear] " << m_DataAll.m_Commands.clearCmd.str << std::endl;
    fil << "[isVisibleButtonClear] " << std::boolalpha << m_DataAll.m_Commands.clearCmd.isVisibleButton << std::endl;
    fil << "[isLoadClear] " << std::boolalpha << m_DataAll.m_Commands.clearCmd.isLoad << std::endl;

    fil << "[Clean] " << m_DataAll.m_Commands.cleanCmd.str << std::endl;
    fil << "[isVisibleButtonClean] " << std::boolalpha << m_DataAll.m_Commands.cleanCmd.isVisibleButton << std::endl;
    fil << "[isLoadClean] " << std::boolalpha << m_DataAll.m_Commands.cleanCmd.isLoad << std::endl;

    fil << "[Str] " << m_DataAll.m_Commands.strCmd.str << std::endl;
    fil << "[isVisibleButtonStr] " << std::boolalpha << m_DataAll.m_Commands.strCmd.isVisibleButton << std::endl;
    fil << "[isLoadStr] " << std::boolalpha << m_DataAll.m_Commands.strCmd.isLoad << std::endl;

    fil << "[Agi] " << m_DataAll.m_Commands.agiCmd.str << std::endl;
    fil << "[isVisibleButtonAgi] " << std::boolalpha << m_DataAll.m_Commands.agiCmd.isVisibleButton << std::endl;
    fil << "[isLoadAgi] " << std::boolalpha << m_DataAll.m_Commands.agiCmd.isLoad << std::endl;

    fil << "[Int] " << m_DataAll.m_Commands.intCmd.str << std::endl;
    fil << "[isVisibleButtonInt] " << std::boolalpha << m_DataAll.m_Commands.intCmd.isVisibleButton << std::endl;
    fil << "[isLoadInt] " << std::boolalpha << m_DataAll.m_Commands.intCmd.isLoad << std::endl;

    fil.close();
}

void LoadCommands::parseCommandLine(const std::string& line) {
    std::istringstream iss(line);
    std::string firstWord;
    if (iss >> firstWord) {
        auto it = m_DataAll.m_Commands.commandMap.find(firstWord);
        if (it != m_DataAll.m_Commands.commandMap.end()) {
            std::string restOfLine;
            std::getline(iss, restOfLine);
            if (!restOfLine.empty() && restOfLine[0] == ' ') {
                restOfLine.erase(0, 1);
            }
            it->second->str = restOfLine;
        }
        else {
            std::string booleanValue;
            std::getline(iss, booleanValue);
            if (!booleanValue.empty() && booleanValue[0] == ' ') {
                booleanValue.erase(0, 1);
            }
            if (firstWord.find("[isVisibleButton") != std::string::npos) {
                for (auto& cmd : m_DataAll.m_Commands.commandMap) {
                    if (firstWord == "[isVisibleButton" + cmd.first.substr(1)) {
                        std::istringstream(booleanValue) >> std::boolalpha >> cmd.second->isVisibleButton;
                        break;
                    }
                }
            }
            else if (firstWord.find("[isLoad") != std::string::npos) {
                for (auto& cmd : m_DataAll.m_Commands.commandMap) {
                    if (firstWord == "[isLoad" + cmd.first.substr(1)) {
                        std::istringstream(booleanValue) >> std::boolalpha >> cmd.second->isLoad;
                        break;
                    }
                }
            }
        }
    }
}

bool LoadCommands::starCommands() {
    getMapOpen getMapOpen_;
    m_FileName = getMapOpen_.getMapOpen1(m_DataAll.m_DataPath.getMapsPath());
    if (m_FileName == L"Error") return false;

    m_DataAll.m_DataMaps.NameMaps(m_FileName);
    const std::wstring putSaveCode = m_DataAll.m_DataMaps.getNameMaps();
    m_FullPath = m_putDatMapPut + L"\\" + putSaveCode;
    std::filesystem::create_directory(m_FullPath);

    std::wstring str = m_FullPath;
    m_FullPath += m_ComandsPath;
    if (std::filesystem::exists(m_FullPath.c_str())) {  // Проверка существования файла
        load();
        std::filesystem::remove(m_FullPath.c_str());    // Удаление файла
    }
    m_FullPath = str + m_CommandsPath;
    
    return true;
}

void LoadCommands::saveCommands() {
    if (!starCommands()) {
        m_DataAll.m_Commands.load = false;
        return;
    }

    writeDefaultCommands(m_FullPath);
    m_DataAll.m_Commands.load = true;
}

void LoadCommands::loadCommands() {
    if (!starCommands()) {
        m_DataAll.m_Commands.load = false;
    }

    load();
    m_DataAll.m_Commands.load = true;
}

void LoadCommands::load() {
    std::ifstream file(m_FullPath.c_str());
    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        writeDefaultCommands(m_FullPath);
    }
    else {
        std::string line;
        while (std::getline(file, line)) {
            parseCommandLine(line);
        }
        file.close();
    }
}
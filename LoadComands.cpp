#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "LoadComands.h"
#include "getMapOpen.h"

//void LoadComands::writeDefaultCommands(const std::wstring& fullPath, Comands& comands) {
//    std::ofstream fil(fullPath.c_str());
//    for (const auto& cmd : comands.commandMap) {
//        fil << cmd.first << " " << cmd.second->str << std::endl;
//        fil << "[isVisibleButton" << cmd.first.substr(1) << " " << std::boolalpha << cmd.second->isVisibleButton << std::endl;
//        fil << "[isLoad" << cmd.first.substr(1) << " " << std::boolalpha << cmd.second->isLoad << std::endl;
//    }
//}
void LoadComands::writeDefaultCommands(const std::wstring& fullPath, Comands& comands) {
    std::ofstream fil(fullPath.c_str());
    fil << "[Return] " << comands.returnCmd.str << std::endl;
    fil << "[isVisibleButtonReturn] " << std::boolalpha << comands.returnCmd.isVisibleButton << std::endl;
    fil << "[isLoadReturn] " << std::boolalpha << comands.returnCmd.isLoad << std::endl;

    fil << "[Save] " << comands.saveCmd.str << std::endl;
    fil << "[isVisibleButtonSave] " << std::boolalpha << comands.saveCmd.isVisibleButton << std::endl;
    fil << "[isLoadSave] " << std::boolalpha << comands.saveCmd.isLoad << std::endl;

    fil << "[Craft] " << comands.craftCmd.str << std::endl;
    fil << "[isVisibleButtonCraft] " << std::boolalpha << comands.craftCmd.isVisibleButton << std::endl;
    fil << "[isLoadCraft] " << std::boolalpha << comands.craftCmd.isLoad << std::endl;

    fil << "[Cam] " << comands.camCmd.str << std::endl;
    fil << "[isVisibleButtonCam] " << std::boolalpha << comands.camCmd.isVisibleButton << std::endl;
    fil << "[isLoadCam] " << std::boolalpha << comands.camCmd.isLoad << std::endl;

    fil << "[Stats] " << comands.statsCmd.str << std::endl;
    fil << "[isVisibleButtonStats] " << std::boolalpha << comands.statsCmd.isVisibleButton << std::endl;
    fil << "[isLoadStats] " << std::boolalpha << comands.statsCmd.isLoad << std::endl;

    fil << "[Clear] " << comands.clearCmd.str << std::endl;
    fil << "[isVisibleButtonClear] " << std::boolalpha << comands.clearCmd.isVisibleButton << std::endl;
    fil << "[isLoadClear] " << std::boolalpha << comands.clearCmd.isLoad << std::endl;

    fil << "[Clean] " << comands.cleanCmd.str << std::endl;
    fil << "[isVisibleButtonClean] " << std::boolalpha << comands.cleanCmd.isVisibleButton << std::endl;
    fil << "[isLoadClean] " << std::boolalpha << comands.cleanCmd.isLoad << std::endl;

    fil << "[Str] " << comands.strCmd.str << std::endl;
    fil << "[isVisibleButtonStr] " << std::boolalpha << comands.strCmd.isVisibleButton << std::endl;
    fil << "[isLoadStr] " << std::boolalpha << comands.strCmd.isLoad << std::endl;

    fil << "[Agi] " << comands.agiCmd.str << std::endl;
    fil << "[isVisibleButtonAgi] " << std::boolalpha << comands.agiCmd.isVisibleButton << std::endl;
    fil << "[isLoadAgi] " << std::boolalpha << comands.agiCmd.isLoad << std::endl;

    fil << "[Int] " << comands.intCmd.str << std::endl;
    fil << "[isVisibleButtonInt] " << std::boolalpha << comands.intCmd.isVisibleButton << std::endl;
    fil << "[isLoadInt] " << std::boolalpha << comands.intCmd.isLoad << std::endl;

    fil.close();
}

void LoadComands::parseCommandLine(const std::string& line, Comands& comands) {
    std::istringstream iss(line);
    std::string firstWord;
    if (iss >> firstWord) {
        auto it = comands.commandMap.find(firstWord);
        if (it != comands.commandMap.end()) {
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
                for (auto& cmd : comands.commandMap) {
                    if (firstWord == "[isVisibleButton" + cmd.first.substr(1)) {
                        std::istringstream(booleanValue) >> std::boolalpha >> cmd.second->isVisibleButton;
                        break;
                    }
                }
            }
            else if (firstWord.find("[isLoad") != std::string::npos) {
                for (auto& cmd : comands.commandMap) {
                    if (firstWord == "[isLoad" + cmd.first.substr(1)) {
                        std::istringstream(booleanValue) >> std::boolalpha >> cmd.second->isLoad;
                        break;
                    }
                }
            }
        }
    }
}

bool LoadComands::starComands() {
    getMapOpen getMapOpen_;
    m_FileName = getMapOpen_.getMapOpen1(m_DataPath.getMapsPath());
    if (m_FileName == L"Error") return false;

    m_DataMaps.NameMaps(m_FileName);
    const std::wstring putSaveCode = m_DataMaps.getNameMaps();
    m_FullPath = m_putDatMapPut + L"\\" + putSaveCode;
    std::filesystem::create_directory(m_FullPath);
    m_FullPath += m_ComandsPath;
    
    return true;
}

void LoadComands::saveComands(Comands& comands) {
    if (!starComands()) {
        comands.load = false;
        return;
    }

    writeDefaultCommands(m_FullPath, comands);
    comands.load = true;
}


Comands LoadComands::loadComands() {
    Comands comands;
    if (!starComands()) {
        comands.load = false;
        return comands;
    }

    std::ifstream file(m_FullPath.c_str());
    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        writeDefaultCommands(m_FullPath, comands);
    }
    else {
        std::string line;
        while (std::getline(file, line)) {
            parseCommandLine(line, comands);
        }
        file.close();
    }
    comands.load = true;
    return comands;
}
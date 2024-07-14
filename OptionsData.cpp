#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

#include "OptionsData.h"

OptionsData::OptionsData()
{
    std::ifstream file(m_FullPath.c_str());

    // ���������, ������ �� ����
    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        // ������� ����, ���� �� �� ����������, � ���������� � ���� �������� �� ���������
        saveFileOptions();
        return; // ������� �� ������������, ��� ��� ���� ��� ������ ������
    }

    // ���������� ������� � ������� ����� ��� �� �������� � ����������
    

    std::string line;
    std::unordered_map<std::string, std::string> fileContent;

    // ������ ���� ��������� � ��������� ����������
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string firstWord;
        // �������� ������ ����� � ������
        if (iss >> firstWord) {
            std::string restOfLine;
            std::getline(iss, restOfLine);
            // ������� ������� ������� �� ���������� ����� ������, ���� ����������
            if (!restOfLine.empty() && restOfLine[0] == ' ') {
                restOfLine.erase(0, 1);
            }
            fileContent[firstWord] = restOfLine;
        }
    }

    file.close();

    bool shouldRewrite = false;
    if (!fileContent.empty()) {
        // ��������� ���������� � ������������� ��������
        for (auto& data : m_DataOptions) {
            if (fileContent.find(data.first) != fileContent.end()) {
                const std::string& value = fileContent[data.first];
                if (value == "true") {
                    if(data.first == "[autoSkillsUpgrade]")
                        data.second = false;
                    else
                        data.second = true;
                }
                else if (value == "false") {
                    data.second = false;
                }
                else {
                    // ���� �������� �� "true" � �� "false", ���������� �������� �� ���������
                    shouldRewrite = true;
                    /*if (command == "[auto�lickerKey]") {
                        *m_CommandMap[command] = auto�lickerKey;
                    }
                    else if (command == "[auto�lickerMaus]") {
                        *m_CommandMap[command] = auto�lickerMaus;
                    }
                    else if (command == "[autoSave]") {
                        *m_CommandMap[command] = autoSave;
                    }
                    else if (command == "[autoStart]") {
                        *m_CommandMap[command] = autoStart;
                    }
                    else if (command == "[autoSkillsUpgrade]") {
                        *m_CommandMap[command] = autoSkillsUpgrade;
                    }*/
                }
            }
        }
    }
    else
        shouldRewrite = true;

    // ���� ����������, ������������ ���� � ���������� �� ���������
    if (shouldRewrite) {
        saveFileOptions();
    }
}

void OptionsData::saveFileOptions() const
{
    std::ofstream fileOut(m_FullPath.c_str());
    if (fileOut.is_open()) {
        for(const auto& data : m_DataOptions)
            fileOut << data.first + " " << (data.second ? "true" : "false") << std::endl;
        //fileOut << "[auto�lickerMaus] " << (auto�lickerMaus ? "true" : "false") << std::endl;
        //fileOut << "[autoSave] " << (autoSave ? "true" : "false") << std::endl;
        //fileOut << "[autoStart] " << (autoStart ? "true" : "false") << std::endl;
        //fileOut << "[autoSkillsUpgrade] " << (autoSkillsUpgrade ? "true" : "false") << std::endl;
        fileOut.close();
    }
}

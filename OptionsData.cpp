#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

#include "OptionsData.h"

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
    const std::vector<std::string> commands = { "[auto�lickerKey]", "[auto�lickerMaus]", "[autoStart]"};
    std::unordered_map<std::string, bool*> commandMap = {
        {"[auto�lickerKey]", &auto�lickerKey},
        {"[auto�lickerMaus]", &auto�lickerMaus},
        {"[autoStart]", &autoStart}
    };

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

    // ��������� ���������� � ������������� ��������
    bool shouldRewrite = false;
    for (const auto& command : commands) {
        if (fileContent.find(command) != fileContent.end()) {
            const std::string& value = fileContent[command];
            if (value == "true") {
                *commandMap[command] = true;
            }
            else if (value == "false") {
                *commandMap[command] = false;
            }
            else {
                // ���� �������� �� "true" � �� "false", ���������� �������� �� ���������
                shouldRewrite = true;
                if (command == "[auto�lickerKey]") {
                    *commandMap[command] = auto�lickerKey;
                }
                else if (command == "[auto�lickerMaus]") {
                    *commandMap[command] = auto�lickerMaus;
                }
                else if (command == "[autoStart]") {
                    *commandMap[command] = autoStart;
                }
            }
        }
    }

    // ���� ����������, ������������ ���� � ���������� �� ���������
    if (shouldRewrite) {
        saveFileOptions();
    }
}

void OptionsData::saveFileOptions() const
{
    std::ofstream fileOut(m_FullPath.c_str());
    if (fileOut.is_open()) {
        fileOut << "[auto�lickerKey] " << (auto�lickerKey ? "true" : "false") << std::endl;
        fileOut << "[auto�lickerMaus] " << (auto�lickerMaus ? "true" : "false") << std::endl;
        fileOut << "[autoStart] " << (autoStart ? "true" : "false") << std::endl;
        fileOut.close();
    }
}

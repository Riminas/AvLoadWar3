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

    // Проверяем, открыт ли файл
    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        // Создаем файл, если он не существует, и записываем в него значения по умолчанию
        saveFileOptions();
        return; // Выходим из конструктора, так как файл был создан заново
    }

    // Определяем команды и создаем карту для их привязки к переменным
    

    std::string line;
    std::unordered_map<std::string, std::string> fileContent;

    // Читаем файл построчно и сохраняем содержимое
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string firstWord;
        // Получаем первое слово в строке
        if (iss >> firstWord) {
            std::string restOfLine;
            std::getline(iss, restOfLine);
            // Удаляем ведущие пробелы из оставшейся части строки, если необходимо
            if (!restOfLine.empty() && restOfLine[0] == ' ') {
                restOfLine.erase(0, 1);
            }
            fileContent[firstWord] = restOfLine;
        }
    }

    file.close();

    bool shouldRewrite = false;
    if (!fileContent.empty()) {
        // Проверяем содержимое и устанавливаем значения
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
                    // Если значение не "true" и не "false", записываем значение по умолчанию
                    shouldRewrite = true;
                    /*if (command == "[autoСlickerKey]") {
                        *m_CommandMap[command] = autoСlickerKey;
                    }
                    else if (command == "[autoСlickerMaus]") {
                        *m_CommandMap[command] = autoСlickerMaus;
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

    // Если необходимо, переписываем файл с значениями по умолчанию
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
        //fileOut << "[autoСlickerMaus] " << (autoСlickerMaus ? "true" : "false") << std::endl;
        //fileOut << "[autoSave] " << (autoSave ? "true" : "false") << std::endl;
        //fileOut << "[autoStart] " << (autoStart ? "true" : "false") << std::endl;
        //fileOut << "[autoSkillsUpgrade] " << (autoSkillsUpgrade ? "true" : "false") << std::endl;
        fileOut.close();
    }
}

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

    // Проверяем, открыт ли файл
    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        // Создаем файл, если он не существует, и записываем в него значения по умолчанию
        saveFileOptions();
        return; // Выходим из конструктора, так как файл был создан заново
    }

    // Определяем команды и создаем карту для их привязки к переменным
    const std::vector<std::string> commands = { "[autoСlickerKey]", "[autoСlickerMaus]", "[autoStart]"};
    std::unordered_map<std::string, bool*> commandMap = {
        {"[autoСlickerKey]", &autoСlickerKey},
        {"[autoСlickerMaus]", &autoСlickerMaus},
        {"[autoStart]", &autoStart}
    };

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

    // Проверяем содержимое и устанавливаем значения
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
                // Если значение не "true" и не "false", записываем значение по умолчанию
                shouldRewrite = true;
                if (command == "[autoСlickerKey]") {
                    *commandMap[command] = autoСlickerKey;
                }
                else if (command == "[autoСlickerMaus]") {
                    *commandMap[command] = autoСlickerMaus;
                }
                else if (command == "[autoStart]") {
                    *commandMap[command] = autoStart;
                }
            }
        }
    }

    // Если необходимо, переписываем файл с значениями по умолчанию
    if (shouldRewrite) {
        saveFileOptions();
    }
}

void OptionsData::saveFileOptions() const
{
    std::ofstream fileOut(m_FullPath.c_str());
    if (fileOut.is_open()) {
        fileOut << "[autoСlickerKey] " << (autoСlickerKey ? "true" : "false") << std::endl;
        fileOut << "[autoСlickerMaus] " << (autoСlickerMaus ? "true" : "false") << std::endl;
        fileOut << "[autoStart] " << (autoStart ? "true" : "false") << std::endl;
        fileOut.close();
    }
}

#include <iostream>
#include "getMapOpen.h"
#include <regex>

std::string getMapOpen::getMapOpen1(const std::string& folder_path) 
{
    if (std::filesystem::is_directory(folder_path.c_str())) {

        std::vector<std::thread> threads;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(folder_path)) {
            if (std::filesystem::is_directory(entry.path())) {
                continue; // Пропускаем директории, если это необходимо
            }
            threads.emplace_back([&](const std::filesystem::path& path) {
                checkFile(path, m_nameFile);
                }, entry.path());
        }

        for (auto& t : threads) {
            t.join();
        }
        if (!m_nameFile.empty()) {
            std::ifstream fil(m_nameFile);
            std::string str;
            getline(fil, str);

            if (str.substr(0, 4) == "HM3W") str = str.substr(5);
            int i = 0;
            while (str[i] == '\0') { i++; }
            str = str.substr(i);

            i = 0;
            while (str[i] != '\0') { i++; }
            str = str.substr(0, i);

            for (int j = 0; j < str.size(); j++) {
                if (str[j] == '|') {
                    if (str[j + 1] == 'r' || str[j + 1] == 'R') {
                        str = str.substr(0, j) + str.substr(j+2);
                        break;
                    }
                    else if (str[j+1] == 'c' || str[j+1] == 'C') {
                        str = str.substr(0, j) + str.substr(j + 10);
                    }
                }
            }

            if (containsOnlyEnglishCharacters(str)) {
                return str;
            }
            else {
                std::filesystem::path path = m_nameFile;
                return path.filename().string();
            }

        }
        else {
            std::cout << "Error: (not \"m_nameFile\") - (" + m_nameFile + ")" << std::endl;
        }
    }
    else {
        std::cout << "Error: Put(not \"Maps\" directory) - ( " << folder_path << " )" << std::endl;
    }
    return "Error";
}

void getMapOpen::checkFile(const std::filesystem::path& filePath, std::string& nameFile) {
    std::fstream file;
    file.open(filePath);
    if (!file.is_open()) {
        std::lock_guard<std::mutex> lock(m_Mtx);
        std::filesystem::path p(filePath);
        nameFile = p.string();
    }
    file.close();
}

bool getMapOpen::containsOnlyEnglishCharacters(const std::string& text) {
    // Регулярное выражение для проверки английских символов, цифр, пробелов, точек и дефисов
    std::regex englishRegex("^[A-Za-z0-9 .\\$\\;\\@\\!\\^\\+\\=\\/\\<\\>\\#\\?\\*\\,\\\"\\\'\\&\\:\\_\\~\\(\\)\\-\\[\\]]*$");
    return std::regex_match(text, englishRegex);
}
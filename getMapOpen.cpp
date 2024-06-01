#include <iostream>
#include "getMapOpen.h"

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
        if (!m_nameFile.empty())
            return m_nameFile;
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
        nameFile = p.filename().string();
    }
    file.close();
}
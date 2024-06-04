#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

class DataPath
{
public:
    struct HeroInfo
    {
        std::string nameChar = "\0";
        std::string path = "\0";
        std::filesystem::file_time_type latestTime = std::filesystem::file_time_type();

        HeroInfo() = default;
        HeroInfo(const std::string& name, const std::string& path, const std::filesystem::file_time_type& time)
            : nameChar(name), path(path), latestTime(time) {}
    };

    bool initializePaths();
    std::vector<HeroInfo> retrieveHeroData(const std::string& saveCodePath, bool& isError);

    std::string getMapsPath() const;
    std::string getSavePath() const;
    std::vector<HeroInfo> getHeroList() const;

private:
    std::vector<HeroInfo> parseHeroData(const std::string& folderPath);
    void processDirectory(const std::filesystem::directory_entry& dirEntry, std::unordered_map<std::string, HeroInfo>& heroDataMap);
    void processFile(const std::filesystem::directory_entry& fileEntry, std::unordered_map<std::string, HeroInfo>& heroDataMap);

    bool isTextFile(const std::filesystem::directory_entry& entry) const;
    bool isHeroToken(const std::string& token) const;
    std::string extractHeroName(const std::vector<std::string>& tokens, size_t startIndex) const;
    void updateHeroInfo(const std::filesystem::directory_entry& fileEntry, HeroInfo& heroInfo);
    std::vector<HeroInfo> convertToHeroList(const std::unordered_map<std::string, HeroInfo>& heroDataMap) const;

    std::vector<HeroInfo> m_HeroList;
    std::string m_Maps = "\0";
    std::string m_Save = "\0";
};



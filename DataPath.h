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
        std::wstring nameChar = L"\0";
        std::wstring path = L"\0";
        std::filesystem::file_time_type latestTime = std::filesystem::file_time_type();

        HeroInfo() = default;
        HeroInfo(const std::wstring& name, const std::wstring& path, const std::filesystem::file_time_type& time)
            : nameChar(name), path(path), latestTime(time) {}
    };



    bool initializePaths();
    std::vector<HeroInfo> retrieveHeroData(const std::wstring& saveCodePath, bool& isError);

    std::wstring getMapsPath() const;
    std::wstring getSavePath() const;
    std::vector<HeroInfo> getHeroList() const;

private:
    std::wstring openWarcraft3();

    void sortList(std::vector<DataPath::HeroInfo>& heroList);
    std::vector<HeroInfo> parseHeroData(const std::wstring& folderPath);
    void processDirectory(const std::filesystem::directory_entry& dirEntry, std::unordered_map<std::wstring, HeroInfo>& heroDataMap);
    void processFile(const std::filesystem::directory_entry& fileEntry, std::unordered_map<std::wstring, HeroInfo>& heroDataMap);

    bool isTextFile(const std::filesystem::directory_entry& entry) const;
    bool isHeroToken(const std::wstring& token) const;
    bool isHeroTokenIsTrue(const std::wstring& token) const;
    std::wstring extractHeroName(const std::vector<std::wstring>& tokens, size_t startIndex) const;
    void updateHeroInfo(const std::filesystem::directory_entry& fileEntry, HeroInfo& heroInfo);
    std::vector<HeroInfo> convertToHeroList(const std::unordered_map<std::wstring, HeroInfo>& heroDataMap) const;

    std::vector<HeroInfo> m_HeroList;
    std::wstring m_Maps = L"\0";
    std::wstring m_Save = L"\0";
};



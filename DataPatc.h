#pragma once
#include <filesystem>
#include <string>
class DataPatc
{
public:
	struct listHero
	{
		std::string nameChar = "\0";
		std::string patc = "\0";
		std::filesystem::file_time_type latestTime = std::filesystem::file_time_type();
	};

	bool loadDataOptions();
	std::vector<listHero> getFileSaveCode1(const std::string& t_PutSaveCode, bool& isError);

	std::string getMaps();
	std::string getSave();
	std::vector<listHero> getListHero1();
private:
	std::vector<listHero> lastFail(const std::string& t_PutSaveCode);
	//std::string lastFail(const std::string& t_PutSaveCode) const;

	std::vector<listHero> m_ListHero1;
	std::string m_Maps = "\0";
	std::string m_Save = "\0";
};


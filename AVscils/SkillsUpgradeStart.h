#pragma once
#include <string>
#include <vector>
class SkillsUpgradeStart
{
public:
	SkillsUpgradeStart(const std::wstring& t_NameMaps);
	void skillsUpgradeStart();
private:
	bool loadDataSkill();

	bool charToAscii(int& c);
	
	std::vector<std::pair<int, unsigned>> m_DataSkill;

	std::wstring m_FullPath = L"\0";
	const std::wstring& m_NameMaps;
	const std::wstring m_pathDatMap = L"DataMaps";
	const std::wstring m_pathFile = L"\\Skill.txt";
};


#pragma once
#include <chrono>

struct OptionsData
{
	OptionsData();
	bool auto—lickerKey = true;
	bool auto—lickerMaus = true;
	bool autoStart = false;
	bool autoSave = false;
	bool autoSkillsUpgrade = false;


	bool isStaitAuto = false;
	float timeDelay{ 600000 };
	std::chrono::steady_clock::time_point lastSaveTime = std::chrono::high_resolution_clock::now();

	void saveFileOptions() const;
private:
	const std::string m_FullPath = "dataAvLoad\\OptionsData.ini";
	std::vector<std::pair<std::string, bool&>> m_DataOptions = {
		{"[auto—lickerKey]", auto—lickerKey},
		{"[auto—lickerMaus]", auto—lickerMaus},
		{"[autoSave]", autoSave},
		{"[autoStart]", autoStart},
		{"[autoSkillsUpgrade]", autoSkillsUpgrade},
	};
};
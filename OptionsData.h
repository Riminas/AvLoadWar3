#pragma once

struct OptionsData
{
	OptionsData();
	bool auto—lickerKey = true;
	bool auto—lickerMaus = true;
	bool autoStart = false;

	void saveFileOptions() const;
private:
	const std::string m_FullPath = "dataAvLoad\\OptionsData.ini";
};
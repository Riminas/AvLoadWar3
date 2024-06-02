#pragma once
#include "DataMaps.h"
#include "DataPatc.h"
class EngineFile
{
public:
	void engineFile1(bool isLastSaveCode);
private:
	void engineTip1(const bool& isLastSaveCode);
	void engineTip2();
	bool isProcessWarcraftIII();

	std::string file_time_to_string(const std::filesystem::file_time_type& ftime);

	std::vector<DataPatc::listHero> m_ListHero;
	std::string m_FileName = "\0";
	std::string m_PatcFile = "\0";
	DataPatc m_DataPatc;
	DataMaps m_DataMaps;
};


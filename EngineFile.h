#pragma once
#include "DataMaps.h"
#include "DataPath.h"
class EngineFile
{
public:
	EngineFile(DataPath& t_DataPatc) : m_DataPath(t_DataPatc) {};
	void engineFile1(bool isLastSaveCode);
private:
	void engineTip1(const bool& isLastSaveCode);
	void engineTip2();
	bool isProcessWarcraftIII();

	std::string file_time_to_string(const std::filesystem::file_time_type& ftime);

	std::vector<DataPath::HeroInfo> m_ListHero;
	std::string m_FileName = "\0";
	std::string m_PatcFile = "\0";
	DataPath& m_DataPath;
	DataMaps m_DataMaps;
};


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
	bool createFileDataMaps();
	bool isProcessWarcraftIII();

	std::vector<DataPatc::listHero> m_ListHero;
	std::string m_FileName = "\0";
	std::string m_PatcFile = "\0";
	DataPatc m_DataPatc;
	DataMaps m_DataMaps;
};


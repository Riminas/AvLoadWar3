#pragma once
#include <string>
class DataMaps
{
public:
	bool loadDataMaps(const std::string& fileName);
	std::string getPutSaveCode();
	std::string getNameMaps();
	std::string getPutDatMapPut();
private:
	void NameMaps(const std::string& fileName);
	bool PutSaveCode();
	bool loadDatFail();

	std::string m_PutSaveCode = "\0";
	std::string m_NameMaps = "\0";

	const std::string m_putDatMapPut = "dataAvLoad\\DataMaps";
};


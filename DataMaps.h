#pragma once
#include <string>
class DataMaps
{
public:
	void NameMaps(const std::string& fileName);
	bool PutSaveCode(const std::wstring& path);


	std::string getPutSaveCode() const;
	std::string getNameMaps() const;
	std::string getPutDatMapPut() const;
private:
	bool loadDatFail();
	std::string removePath(const std::string& filePath);
	bool PathDirectorySaveCode(const std::wstring& path);

	std::string m_PutSaveCode = "\0";
	std::string m_NameMaps = "\0";

	const std::string m_putDatMapPut = "dataAvLoad\\DataMaps";
};


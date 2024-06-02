#pragma once
#include <string>
class DataMaps
{
public:
	bool loadDataMaps(const std::string& fileName, const std::wstring& path);
	std::string getPutSaveCode() const;
	std::string getNameMaps() const;
	std::string getPutDatMapPut() const;
private:
	void NameMaps(const std::string& fileName);
	bool PutSaveCode(const std::wstring& path);
	bool loadDatFail();

	std::wstring BrowseFolderDialog(const std::wstring& initialPath);
	std::string removePath(const std::string& filePath);

	std::string m_PutSaveCode = "\0";
	std::string m_NameMaps = "\0";

	const std::string m_putDatMapPut = "dataAvLoad\\DataMaps";
};


#pragma once
#include <string>
class DataMaps
{
public:
	void NameMaps(const std::wstring& fileName);
	int PutSaveCode(const std::wstring& path);

	std::wstring getPutSaveCode() const;
	std::wstring getNameMaps() const;
	std::wstring getPutDatMapPut() const;

	std::wstring m_PutSaveCode = L"\0";
private:
	bool loadDatFail(const std::wstring& fullPath);
	std::wstring utf8_to_utf16(const std::string& str);
	std::wstring removePath(const std::wstring& filePath);
	bool PathDirectorySaveCode(const std::wstring& path);

	std::wstring m_NameMaps = L"\0";

	const std::wstring m_pathDatMap = L"DataMaps";
	const std::wstring m_pathFile = L"\\Path.dat";
};


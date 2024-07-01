#pragma once
#include <string>
class StringToString
{
public:
	static std::vector<std::wstring> removeString(const std::wstring& t_fileName);
private:
	static std::wstring removeOnString(std::wstring& t_wstr);

	static bool isIsclu(const std::wstring& word);
};


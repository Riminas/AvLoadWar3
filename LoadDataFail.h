#pragma once
#include <string>
class LoadDataFail
{
public:
	std::wstring loadDataFail(const std::wstring& pathFail);
	std::vector<std::wstring> loadDataFailTokens(const std::wstring& pathFail);
};


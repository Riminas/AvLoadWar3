#pragma once
class StringToString
{
public:
	static std::vector<std::string> removeString(const std::string& t_fileName);
	std::string strVectorToStr(const std::string& t_fileName);
	void testStringComparison(const std::vector<std::pair<std::pair<std::string, std::string>, bool>>& testCases);
private:
	static std::string removeOnString(const std::string& t_str);
	static bool isIsclu(const std::string& word);
};


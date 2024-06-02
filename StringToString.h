#pragma once
class StringToString
{
public:
	static std::vector<std::string> removeString(const std::string& t_fileName);
	static bool isMap(const std::string& str1, const std::string& str2);
	void testStringComparison(const std::vector<std::pair<std::pair<std::string, std::string>, bool>>& testCases);
private:
	static std::string removeOnString(const std::string& t_str);
	static bool isIsclu(const std::string& word);
};


#pragma once
class StringToString
{
public:
	bool isMap(std::string str1, std::string str2);
	bool isMapCoutInformtion(std::string str1, std::string str2);
	std::vector<std::string> removeString(const std::string& t_fileName);
private:
	bool isIsclu(const std::string& word);
	std::string removeOnString(const std::string t_str);
};


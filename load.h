#pragma once
#include <string>
#include <wtypes.h>

class load
{
public:
	load();
	bool load1(const std::string& put);
	void convectorCharKey(const std::string& code);
	void coutCommands(const std::string& strComand);
protected:
	bool codeAllProv(std::vector<std::string>& codeAll);
	void provOne(std::string& code, const std::vector<std::string>& tokens, size_t index, const std::string& prefix = "\0");
private:
	HWND m_targetWindow;
	short m_TipLoad = 0;
	std::string m_Put = "\0";
};


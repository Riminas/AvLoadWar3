#pragma once
#include <SFML/Graphics.hpp>
#include "DataMaps.h"
#include "DataPath.h"
#include "OptionsData.h"



class EngineFileTip1
{
public:
	EngineFileTip1(DataPath& t_DataPath, sf::RenderWindow& t_window, sf::Font& t_font, OptionsData& t_OptionsData);
	void engineFile();
private:
	void engineTip1();

	std::vector<DataPath::HeroInfo> m_ListHero;
	std::wstring m_FileName = L"\0";

	DataPath& m_DataPath;
	DataMaps m_DataMaps;

	sf::RenderWindow& window;
	sf::Font& font;
	OptionsData& m_OptionsData;
};



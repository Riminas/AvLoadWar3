#pragma once
#include <SFML/Graphics.hpp>
#include "DataAll.h"



class EngineFileTip1
{
public:
	EngineFileTip1(sf::RenderWindow& t_window, sf::Font& t_font, DataAll& t_DataAll);
	void engineFile();
private:
	void engineTip1();

	std::vector<DataPath::HeroInfo> m_ListHero;
	std::wstring m_FileName = L"\0";

	sf::RenderWindow& m_Window;
	sf::Font& m_Font;
	DataAll& m_DataAll;
};



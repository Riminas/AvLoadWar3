#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Engine.h"
#include <vector>
#include <Windows.h>

class OpenGuide {
public:
    void openGuide(const std::string& fileName);

private:
    bool loadGuide(const std::wstring& path);
    void initializeTextList(std::vector<sf::Text>& textList, const sf::Font& font, unsigned maxCount);
    void updateTextListNew(std::vector<sf::Text>& textList, const sf::Font& font, int startIndex, unsigned maxCount, bool isNext);
    void updateTextList(std::vector<sf::Text>& textList, int& startIndex, const unsigned& maxCount);
    std::wstring utf8ToWstring(const std::string& str);
    bool isProcessWarcraftIII();
    void setupWindow(sf::RenderWindow& window);
    bool saveStartIndex(const int& startIndex);
    bool loadStartIndex(int& startIndex);
    void messageLoop(sf::RenderWindow& window2, std::vector<sf::Text>& textList, const sf::Font& font, int& startIndex, const unsigned& maxCount, const size_t& guideSize);

    std::vector<std::wstring> m_ListGuide;
    std::string m_FileName;
    const std::wstring m_PathAllGuide = L"Guide";
    const std::wstring m_NameGuide = L"\\Guide.txt";
    const std::wstring m_NameData = L"\\Data.dat";

    bool keyDownW = false;
    bool keyDownS = false;
};



/*
	
	
	как проистываение по гайдам? стрелки
	как найти нужную страку?(быстрый поиск) ............
	
	без быстрокго поиска нет смысла в выводе гайда
	проблемы с читабельностью текста если неделать фон для него


*/
#pragma once
#include <SFML/Graphics.hpp>
#include "DataAll.h"

class EngineFileTip2 {
public:
    EngineFileTip2(sf::RenderWindow& t_Window, sf::Font& t_font, DataAll& t_DataAll);

    int mouseButtonPressed(sf::Event& event, bool& isWindow2Visible);
    void draw();
    short initialize();

    void activeGameTrue(const HWND& hWndWindow, const bool& isUpdataPosition = true);

    std::wstring getPathListHero(const int& i);

    bool isInitialize = false;

private:
    bool engineFile();
    std::wstring file_time_to_wstring(const std::filesystem::file_time_type& ftime);

    void updatePosition(const sf::Vector2f& newPosition);
    void createHeroDraw(int index, unsigned int characterSize);

    sf::RenderWindow& m_Window;
    sf::Font& font;
    DataAll& m_DataAll;

    struct ListHeroDraw {
        sf::RectangleShape shape;
        sf::Text text;
        sf::Text textData;
    };

    std::vector<ListHeroDraw> m_ListHeroDraw;
    std::vector<DataPath::HeroInfo> m_ListHero;

    size_t m_MaxNameWidth = 0;
};
#pragma once
#include <SFML/Graphics.hpp>

struct Button {
    sf::RectangleShape shape;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text text;

    bool isLoadTextur = true;
    bool isVisibleButton = true;
};
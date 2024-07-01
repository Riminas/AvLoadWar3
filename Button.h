#pragma once
#include <SFML/Graphics.hpp>

struct Button {
    sf::RectangleShape shape;
    sf::Texture texture;
    sf::Sprite sprite;

    bool isVisibleButton = true;
};
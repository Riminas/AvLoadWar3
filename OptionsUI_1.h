#pragma once
#include <SFML/Graphics.hpp>


// Структура для хранения данных
struct OptionsUI_1 {
    bool value; // Булевое значение
    sf::RectangleShape square; // Квадрат
    sf::Text checkText; // Текст для крестика или галочки
    sf::Text text; // Основной текст

    // Конструктор для инициализации данных
    void initialize(bool val, const sf::Font& font, const std::string& str, float squareSize, const sf::Color& squareColor, const sf::Color& textColor)
    {
        value = val;
        // Инициализация квадрата
        square.setSize(sf::Vector2f(squareSize, squareSize));
        square.setFillColor(squareColor);

        // Инициализация текста крестика или галочки
        checkText.setFont(font);
        checkText.setString(value ? L"✓" : L"X");
        checkText.setFillColor(textColor);
        checkText.setCharacterSize(static_cast<unsigned int>(squareSize * 0.8f)); // Размер шрифта относительно квадрата

        // Инициализация основного текста
        text.setFont(font);
        text.setString(str);
        text.setFillColor(textColor);
        text.setCharacterSize(static_cast<unsigned int>(squareSize * 0.8f)); // Размер шрифта относительно квадрата
    }

    // Метод для установки позиции
    void setPosition(float x, float y) {
        square.setPosition(x, y);

        // Центрирование текста крестика или галочки внутри квадрата
        sf::FloatRect squareBounds = square.getGlobalBounds();
        sf::FloatRect checkTextBounds = checkText.getLocalBounds();
        checkText.setOrigin(checkTextBounds.left + checkTextBounds.width / 2.0f, checkTextBounds.top + checkTextBounds.height / 2.0f);
        checkText.setPosition(squareBounds.left + squareBounds.width / 2.0f, squareBounds.top + squareBounds.height / 2.0f);

        text.setPosition(x + square.getSize().x + 10, y); // Сдвиг текста вправо на 10 пикселей от квадрата
    }

    void drawRow(sf::RenderWindow& window) {
        window.draw(square);
        window.draw(checkText);
        window.draw(text);
    }

    void updateCheckText() {
        checkText.setString(value ? L"✓" : L"X");
    }
};
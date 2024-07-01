#pragma once
#include <SFML/Graphics.hpp>

// Структура для хранения данных
struct ComandsData {
    bool value1; // Булевое значение для первого квадрата
    bool value2; // Булевое значение для второго квадрата
    sf::RectangleShape square; // Первый квадрат
    sf::RectangleShape square2; // Второй квадрат
    sf::Text checkText; // Текст для крестика или галочки в первом квадрате
    sf::Text checkText2; // Текст для крестика или галочки во втором квадрате
    sf::Text text; // Основной текст
    sf::Text inputText; // Текстовое поле для ввода

    // Конструктор для инициализации данных
    void initialize(bool val1, bool val2, const sf::Font& font, const std::string& str, float squareSize, const sf::Color& squareColor, const sf::Color& textColor)
    {
        value1 = val1;
        value2 = val2;

        // Инициализация первого квадрата
        square.setSize(sf::Vector2f(squareSize, squareSize));
        square.setFillColor(squareColor);

        // Инициализация второго квадрата
        square2.setSize(sf::Vector2f(squareSize, squareSize));
        square2.setFillColor(squareColor);

        // Инициализация текста крестика или галочки в первом квадрате
        checkText.setFont(font);
        checkText.setString(value1 ? L"✓" : L"X");
        checkText.setFillColor(textColor);
        checkText.setCharacterSize(static_cast<unsigned int>(squareSize * 0.8f)); // Размер шрифта относительно квадрата

        // Инициализация текста крестика или галочки во втором квадрате
        checkText2.setFont(font);
        checkText2.setString(value2 ? L"✓" : L"X");
        checkText2.setFillColor(textColor);
        checkText2.setCharacterSize(static_cast<unsigned int>(squareSize * 0.8f)); // Размер шрифта относительно квадрата

        // Инициализация основного текста
        text.setFont(font);
        text.setString(str);
        text.setFillColor(textColor);
        text.setCharacterSize(static_cast<unsigned int>(squareSize * 0.8f)); // Размер шрифта относительно квадрата

        // Инициализация текстового поля для ввода
        inputText.setFont(font);
        inputText.setString(""); // Пустая строка для ввода текста
        inputText.setFillColor(textColor);
        inputText.setCharacterSize(static_cast<unsigned int>(squareSize * 0.8f)); // Размер шрифта относительно квадрата
    }

    // Метод для установки позиции
    void setPosition(float x, float y) {
        square.setPosition(x, y);

        // Центрирование текста крестика или галочки внутри первого квадрата
        sf::FloatRect squareBounds = square.getGlobalBounds();
        sf::FloatRect checkTextBounds = checkText.getLocalBounds();
        checkText.setOrigin(checkTextBounds.left + checkTextBounds.width / 2.0f, checkTextBounds.top + checkTextBounds.height / 2.0f);
        checkText.setPosition(squareBounds.left + squareBounds.width / 2.0f, squareBounds.top + squareBounds.height / 2.0f);

        // Установка позиции второго квадрата правее первого квадрата с расстоянием 30 пикселей
        square2.setPosition(x + square.getSize().x + 90, y);

        // Центрирование текста крестика или галочки внутри второго квадрата
        sf::FloatRect square2Bounds = square2.getGlobalBounds();
        sf::FloatRect checkText2Bounds = checkText2.getLocalBounds();
        checkText2.setOrigin(checkText2Bounds.left + checkText2Bounds.width / 2.0f, checkText2Bounds.top + checkText2Bounds.height / 2.0f);
        checkText2.setPosition(square2Bounds.left + square2Bounds.width / 2.0f, square2Bounds.top + square2Bounds.height / 2.0f);

        // Установка позиции основного текста правее второго квадрата с расстоянием 30 пикселей
        text.setPosition(square2Bounds.left + square2Bounds.width + 115, y);

        // Установка позиции текстового поля для ввода правее основного текста с расстоянием 30 пикселей
        sf::FloatRect textBounds = text.getGlobalBounds();
        inputText.setPosition(textBounds.left + textBounds.width + 30, y);
    }

    void drawComands(sf::RenderWindow& window) {
        //window.draw(square);
        //window.draw(checkText);
        window.draw(square2);
        window.draw(checkText2);
        window.draw(text);
        window.draw(inputText);
    }

    void updateCheckText() {
        checkText.setString(value1 ? L"✓" : L"X");
        checkText2.setString(value2 ? L"✓" : L"X");
    }
};

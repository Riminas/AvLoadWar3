﻿#pragma once
#include <SFML/Graphics.hpp>
#include "StringConvector.h"

// Структура для хранения данных
struct CommandsData {
    bool value1 = true; // Булевое значение для первого квадрата
    bool value2 = false; // Булевое значение для второго квадрата
    bool useSecondSquare = true; // Булевое значение для включения/отключения второго квадрата
    sf::RectangleShape bottonLine; // Линия
    sf::RectangleShape square; // Первый квадрат
    sf::RectangleShape square2; // Второй квадрат
    sf::Text checkText; // Текст для крестика или галочки в первом квадрате
    sf::Text checkText2; // Текст для крестика или галочки во втором квадрате
    sf::Text text; // Основной текст
    sf::Text inputText; // Текстовое поле для ввода

    // Конструктор для инициализации данных
    void initialize(const bool& val1, const bool& val2, const sf::Font& font, const std::string& str, float squareSize, const sf::Color& squareColor, const sf::Color& textColor, bool useSecondSquareFlag)
    {
        value1 = val1;
        value2 = val2;
        useSecondSquare = useSecondSquareFlag;

        // Инициализация основного текста
        text.setFont(font);
        text.setString(str);
        text.setFillColor(textColor);
        text.setCharacterSize(static_cast<unsigned int>(squareSize * 0.8f)); // Размер шрифта относительно квадрата

        // Инициализация первого квадрата
        square.setSize(sf::Vector2f(squareSize, squareSize));
        square.setFillColor(squareColor);

        // Инициализация второго квадрата, если включен
        if (useSecondSquare) {
            square2.setSize(sf::Vector2f(squareSize, squareSize));
            square2.setFillColor(squareColor);

            // Инициализация текста крестика или галочки во втором квадрате
            checkText2.setFont(font);
            checkText2.setString(value2 ? L"✓" : L"X");
            checkText2.setFillColor(textColor);
            checkText2.setCharacterSize(static_cast<unsigned int>(squareSize * 0.8f)); // Размер шрифта относительно квадрата
        }

        // Инициализация текста крестика или галочки в первом квадрате
        checkText.setFont(font);
        checkText.setString(value1 ? L"✓" : L"X");
        checkText.setFillColor(textColor);
        checkText.setCharacterSize(static_cast<unsigned int>(squareSize * 0.8f)); // Размер шрифта относительно квадрата

        // Инициализация текстового поля для ввода
        inputText.setFont(font);
        inputText.setString(""); // Пустая строка для ввода текста
        inputText.setFillColor(textColor);
        inputText.setCharacterSize(static_cast<unsigned int>(squareSize * 0.8f)); // Размер шрифта относительно квадрата

        // Инициализация линии
        bottonLine.setSize(sf::Vector2f(0, 1)); // Устанавливаем начальную ширину равной 0
        bottonLine.setFillColor(sf::Color(128, 128, 128)); // Серый цвет
    }

    // Метод для установки позиции
    void setPosition(float x, float y, float windowWidth, float xOutline) {
        float x2 = x + (windowWidth / 2) + 10;

        // Корректировка текста для помещения в левую половину окна
        StringConvector StringConvector_;
        StringConvector_.adjustTextToFit(text, windowWidth / 2 - xOutline);

        // Установка позиции основного текста
        text.setPosition(x + xOutline, y);

        // Установка позиции первого квадрата правее основного текста с расстоянием 30 пикселей
        sf::FloatRect textBounds = text.getGlobalBounds();
        square.setPosition(x2, y);

        // Центрирование текста крестика или галочки внутри первого квадрата
        sf::FloatRect squareBounds = square.getGlobalBounds();
        sf::FloatRect checkTextBounds = checkText.getLocalBounds();
        checkText.setOrigin(checkTextBounds.left + checkTextBounds.width / 2.0f, checkTextBounds.top + checkTextBounds.height / 2.0f);
        checkText.setPosition(squareBounds.left + squareBounds.width / 2.0f, squareBounds.top + squareBounds.height / 2.0f);

        if (useSecondSquare) {
            // Установка позиции второго квадрата правее первого квадрата с расстоянием 90 пикселей
            square2.setPosition(squareBounds.left + square.getSize().x + 50, y);

            // Центрирование текста крестика или галочки внутри второго квадрата
            sf::FloatRect square2Bounds = square2.getGlobalBounds();
            sf::FloatRect checkText2Bounds = checkText2.getLocalBounds();
            checkText2.setOrigin(checkText2Bounds.left + checkText2Bounds.width / 2.0f, checkText2Bounds.top + checkText2Bounds.height / 2.0f);
            checkText2.setPosition(square2Bounds.left + square2Bounds.width / 2.0f, square2Bounds.top + square2Bounds.height / 2.0f);

            // Установка позиции текстового поля для ввода правее второго квадрата с расстоянием 30 пикселей
            inputText.setPosition(square2Bounds.left + square2Bounds.width + 30, y);
        }
        else {
            // Установка позиции текстового поля для ввода правее первого квадрата с расстоянием 30 пикселей
            inputText.setPosition(squareBounds.left + squareBounds.width + 30, y);
        }

        // Установка позиции и ширины линии
        bottonLine.setSize(sf::Vector2f(windowWidth, 1)); // Устанавливаем ширину линии равной ширине окна
        bottonLine.setPosition(x, y + square.getSize().y + 2); // Линия под квадратами на 2 пикселя ниже
    }

    void drawCommands(sf::RenderWindow& window) {
        window.draw(text); // Отрисовка основного текста
        window.draw(square);
        window.draw(checkText);
        if (useSecondSquare) {
            window.draw(square2);
            window.draw(checkText2);
        }
        window.draw(inputText);
        window.draw(bottonLine); // Отрисовка линии
    }

    void updateCheckText() {
        checkText.setString(value1 ? L"✓" : L"X");
        if (useSecondSquare) {
            checkText2.setString(value2 ? L"✓" : L"X");
        }
    }
};
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include <wtypes.h>
#include "OptionsUI_1.h"
#include "CommandsUI_1.h"
#include "DataAll.h"

struct DirectoryEntry {
    std::wstring name;
    std::wstring fullPath;
    bool isOpen;
    std::vector<DirectoryEntry> subDirectories;

    DirectoryEntry(const std::wstring& name, const std::wstring& fullPath)
        : name(name), fullPath(fullPath), isOpen(false) {}
};

class NewDirectory {
public:
    NewDirectory(sf::RenderWindow& t_Window, sf::Font& font, const sf::Vector2f& newPosition, bool& t_isExetTree, DataAll& t_DataAll);
    int newDirectory();

private:
    void initializeText(sf::Text& text, const std::wstring& textString = L"", const sf::Vector2f& position = { 0,0 }, const sf::Color& color = sf::Color::Black, const float& letterSpacing = 1.0f) const;
    void initializeRectangle(sf::RectangleShape& rectangle, const sf::Vector2f& size = { 10, 10 }, const sf::Vector2f& position = { 0,0 }, const sf::Color& color = sf::Color::White) const;
    std::wstring run();
    void initializeWindow();
    void initializeLine(sf::RectangleShape& line, const sf::Vector2f& size, const sf::Vector2f& position);
    void initializeScrollbar(const float& windowWidth, const float& windowHeight);
    void initializeDataCommands();
    void initializeSettings();
    void initializeTextArray(const std::vector<std::pair<std::string, float>>& dat, unsigned int characterSize);
    void activeMenu();
    void drawWindow();
    std::wstring handleMousePress(sf::Event& event);
    void handleScroll(sf::Event& event);
    void handleMouseMove(sf::Event& event);
    bool IsWindowInFocus(const HWND& hWnd) const;
    void loadDirectories(const std::wstring& directoryPath, DirectoryEntry& parent, bool& isPathSaveCodeFalse);
    bool isPushDirectory(const std::wstring& fullPath, const std::wstring& nameDirectory) const;
    void updateDirectoryTexts();
    void toggleDirectory(DirectoryEntry& directory);
    std::wstring newPath();
    void adjustScrollbar();

    sf::Vector2f newPosition;
    sf::RectangleShape background;
    sf::RectangleShape topBackground;
    sf::RectangleShape bottomBackground;
    sf::Text title;
    sf::RectangleShape topLine;
    sf::RectangleShape bottomLine;
    sf::RectangleShape closeButton;
    sf::Text closeText;
    sf::RectangleShape scrollbar;
    sf::RectangleShape scrollbarThumb;
    sf::Text selectedTextNo;
    sf::Text selectedTextYes;
    sf::RectangleShape directoryButton;
    sf::Text directoryText;
    sf::RectangleShape DataCommandsButton;
    sf::Text DataCommandsText;
    sf::RectangleShape optionsButton;
    sf::Text optionsText;
    sf::RectangleShape bottomLineCommandsUI_1;
    DirectoryEntry rootDirectory;
    sf::Text coutHotKeyText;
    sf::RectangleShape coutHotKeyShape;
    std::vector<sf::Text> directoryTexts;
    std::vector<sf::ConvexShape> directoryTriangles; // Добавлено
    float scrollOffset = 0;
    bool isScrolling = false;
    float scrollSpeed = 1;
    float mouseOffset = 0;
    std::vector<sf::Text> m_TextArray;
    bool isBackgroundDirectory = false;
    sf::RectangleShape backgroundDirectory;
    DirectoryEntry openDirectoryEntry;
    std::wstring nameMaps;
    const std::wstring pathDatMapPut = L"DataMaps";
    const std::wstring pathFile = L"\\Path.dat";
    bool& isExetTree;
    bool m_IsRunning = false;
    int numMenu = 1;
    std::vector<CommandsUI_1> m_CommandsUI_1;
    std::vector<OptionsUI_1> m_OptionsUI_1;
    sf::RenderWindow& window;
    sf::Font& font;
    DataAll& m_DataAll;
};
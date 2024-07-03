#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <functional>
#include <algorithm>
#include <fstream>
#include "LoadCommands.h"
#include "StringConvector.h"

#include "NewDirectory.h"
#include <unordered_set>

NewDirectory::NewDirectory(sf::RenderWindow& t_Window, sf::Font& t_font, const sf::Vector2f& t_newPosition, bool& t_isExetTree, DataAll& t_DataAll) :
    window(t_Window),
    font(t_font),
    newPosition(t_newPosition),
    nameMaps(t_DataAll.m_DataMaps.getNameMaps()),
    rootDirectory(L"CustomMapData", t_DataAll.m_DataPath.getSavePath()),
    isExetTree(t_isExetTree),
    scrollOffset(0),
    isScrolling(false),
    scrollSpeed(1.0f),
    openDirectoryEntry(rootDirectory),
    m_DataAll(t_DataAll)
{
    initializeWindow();
}

inline void NewDirectory::initializeText(sf::Text& text, const std::wstring& textString, const sf::Vector2f& position, const sf::Color& color) const {
    text.setFont(font);
    text.setCharacterSize(16);

    text.setString(textString);
    text.setPosition(position);
    text.setFillColor(color);
}

inline void NewDirectory::initializeRectangle(sf::RectangleShape& rectangle, const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color) const {
    rectangle.setSize(size);
    rectangle.setPosition(position);
    rectangle.setFillColor(color);
}


void NewDirectory::initializeWindow() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    float windowWidth = 500;
    float windowHeight = desktop.height * 2.f / 3.f;
    
    if (newPosition == sf::Vector2f(0, 0))
        newPosition = sf::Vector2f(static_cast<float>(desktop.width) / 2.f - windowWidth / 2.f,
            static_cast<float>(desktop.height) / 2.f - windowHeight / 2.f);

    // Общее меню
    initializeRectangle(background, sf::Vector2f(windowWidth, windowHeight), newPosition);
    initializeText(title, L"Maps: " + nameMaps, newPosition + sf::Vector2f{ 20, 0 });
    initializeLine(topLine, sf::Vector2f(windowWidth, 2), sf::Vector2f(0, 55) + newPosition);
    initializeLine(bottomLine, sf::Vector2f(windowWidth, 2), sf::Vector2f(0, windowHeight - 35) + newPosition);
    initializeRectangle(closeButton, sf::Vector2f(20, 19), sf::Vector2f(windowWidth - 30, -5) + newPosition, sf::Color::Red);
    initializeText(closeText, L"X", newPosition + sf::Vector2f{ windowWidth - 25, -5 }, sf::Color::White);
    initializeText(selectedTextNo, L"Пропустить", newPosition + sf::Vector2f{ 40, windowHeight - 28 });
    initializeText(selectedTextYes, L"Подтвердить выбор", newPosition + sf::Vector2f{ windowWidth / 2, windowHeight - 28 });
    initializeRectangle(topBackground, sf::Vector2f(windowWidth, topLine.getPosition().y - newPosition.y + 3), sf::Vector2f(0, -15) + newPosition);
    initializeRectangle(bottomBackground, sf::Vector2f(windowWidth, 35), sf::Vector2f(newPosition.x, bottomLine.getPosition().y - 1));

    const std::vector<std::pair<std::wstring, std::pair<sf::RectangleShape&, sf::Text&>>> nameRazels = {
        {L"Directory", {directoryButton, directoryText}},
        {L"Commands", {commandsButton, commandsText}},
        {L"Options", {optionsButton, optionsText}},
    };
    const int maxSize = nameRazels.size();

    for (int i = 0; i < maxSize; ++i) {
        const auto& [name, components] = nameRazels[i];
        auto& [button, text] = components;
        initializeRectangle(button, sf::Vector2f(windowWidth / maxSize, 30), newPosition + sf::Vector2f(windowWidth / maxSize * i, 25));
        initializeText(text, name, button.getPosition() + sf::Vector2f{ 20, 5 });
    }

    activeMenu();

    // Меню выбора директории
    initializeScrollbar(windowWidth, windowHeight);

    bool isPathSaveCodeFalse = false;
    const std::wstring& wstr = m_DataAll.m_DataMaps.m_PutSaveCode;
    if (wstr != L"False" && !wstr.empty()) {
        isPathSaveCodeFalse = true;
    }
    loadDirectories(rootDirectory.fullPath, rootDirectory, isPathSaveCodeFalse);

    updateDirectoryTexts();

    // Меню настройки команд
    initializeCommands();

    // Меню настроек
    initializeSettings();

    window.setFramerateLimit(60);
}

void NewDirectory::initializeLine(sf::RectangleShape& line, const sf::Vector2f& size, const sf::Vector2f& position) {
    initializeRectangle(line, size, position, sf::Color::Black);
}

void NewDirectory::initializeScrollbar(const float& windowWidth, const float& windowHeight) {
    scrollbar.setSize(sf::Vector2f(10, windowHeight - 93));
    scrollbar.setPosition(sf::Vector2f(windowWidth - 10, 58) + newPosition);
    scrollbarThumb.setFillColor(sf::Color(100, 100, 100));
    scrollbarThumb.setPosition(scrollbar.getPosition());

    backgroundDirectory.setFillColor(sf::Color(0, 60, 160, 80));
    backgroundDirectory.setSize(sf::Vector2f(windowWidth - 15, 25));

    rootDirectory.isOpen = true;
}


void NewDirectory::initializeCommands() {
    std::vector<std::pair<std::string, sf::Vector2f>> commandConfigs = {
        {"[Return]", {25, 80}},
        {"[Save]", {25, 105}},
        {"[Craft]", {25, 130}},
        {"[Cam]", {25, 155}},
        {"[Stats]", {25, 180}},
        {"[Clear]", {25, 205}},
        {"[Clean]", {25, 230}},
        {"[Str]", {25, 255}},
        {"[Agi]", {25, 280}},
        {"[Int]", {25, 305}},
    };

    m_CommandsData.resize(commandConfigs.size());
    for (size_t i = 0; i < commandConfigs.size(); ++i) {
        auto& [commandName, positionOffset] = commandConfigs[i];
        auto& command = *m_DataAll.m_Commands.commandMap[commandName];
        if(commandName == "[Cam]")
            m_CommandsData[i].initialize(command.isVisibleButton, command.isLoad, font, command.str, 20, sf::Color(180, 180, 180), sf::Color::Black, true);
        else
            m_CommandsData[i].initialize(command.isVisibleButton, command.isLoad, font, command.str, 20, sf::Color(180, 180, 180), sf::Color::Black, false);
        m_CommandsData[i].setPosition(newPosition.x, newPosition.y + positionOffset.y, background.getSize().x, positionOffset.x);
    }

    m_TextArray.resize(3);
    const std::vector<std::pair<std::string, float>> dat{
        {"Commands", 10},
        {"Visible", background.getSize().x/2},
        {"AVCommands", background.getSize().x / 2 + 52},
    };
    initializeTextArray(dat, 12);
}

void NewDirectory::initializeSettings() {
    m_RowData.resize(2);
    const std::vector<std::tuple<std::string, sf::Vector2f>> settings = {
        {"Автонажатия клавишь", {20, 70}},
        {"Автонажатия правой кнопки мыши", {20, 100}},
    };

    for (size_t i = 0; i < settings.size(); ++i) {
        const auto& [label, positionOffset] = settings[i];
        m_RowData[i].initialize(m_DataAll.m_OptionsData.autoСlickerKey, font, label, 24, sf::Color(180, 180, 180), sf::Color::Black);
        m_RowData[i].setPosition(newPosition.x + positionOffset.x, newPosition.y + positionOffset.y);
    }
}

void NewDirectory::initializeTextArray(const std::vector<std::pair<std::string, float>>& dat, unsigned int characterSize)
{
    for (size_t i = 0; i < m_TextArray.size(); ++i)
    {
        m_TextArray[i].setFont(font);
        m_TextArray[i].setString(dat[i].first);
        m_TextArray[i].setCharacterSize(characterSize);
        m_TextArray[i].setFillColor(sf::Color::Black); // Или любой другой цвет, который вам нужен

        m_TextArray[i].setPosition(sf::Vector2f(dat[i].second, 60) + newPosition); // Устанавливаем позицию по X из вектора positionsX и по Y со смещением
    }
    // Установка позиции и ширины линии
    bottomLineCommandsData.setFillColor(sf::Color(128, 128, 128)); // Серый цвет
    bottomLineCommandsData.setSize(sf::Vector2f(background.getSize().x, 1)); // Устанавливаем ширину линии равной ширине окна
    bottomLineCommandsData.setPosition(newPosition.x, newPosition.y + 78); // Линия под квадратами на 2 пикселя ниже
}

void NewDirectory::activeMenu() {
    // Определяем активные и неактивные цвета
    const sf::Color activeColor(100, 100, 250);
    const sf::Color inactiveColor(200, 200, 200);
    const sf::Color activeTextColor(sf::Color::White);
    const sf::Color inactiveTextColor(sf::Color::Black);

    // Массивы кнопок и соответствующих текстов
    std::vector<sf::RectangleShape*> buttons = { &directoryButton, &commandsButton, &optionsButton };
    std::vector<sf::Text*> texts = { &directoryText, &commandsText, &optionsText };

    // Устанавливаем цвета для кнопок и текстов
    for (size_t i = 0; i < buttons.size(); ++i) {
        if (i == numMenu - 1) {
            buttons[i]->setFillColor(activeColor);
            texts[i]->setFillColor(activeTextColor);
        }
        else {
            buttons[i]->setFillColor(inactiveColor);
            texts[i]->setFillColor(inactiveTextColor);
        }
    }
}

void NewDirectory::loadDirectories(const std::wstring& directoryPath, DirectoryEntry& parent, bool& isPathSaveCodeFalse) {
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_directory() && isPushDirectory(entry.path().wstring(), entry.path().filename().wstring())) {
            std::wstring name = entry.path().filename().wstring();

            //StringConvector StringConvector_;
            //const float maxWidth = background.getSize().x - 38; // Максимальная ширина текста
            //StringConvector_.adjustTextToFit(dirText, maxWidth);


            if (name.size() > 35) {
                name.erase(35);
                name += L" ...";
            }
            std::wstring fullPath = entry.path().wstring();
            DirectoryEntry subDir(name, fullPath);
            parent.subDirectories.push_back(subDir);

            if (isPathSaveCodeFalse) {
                const std::wstring& wstr = m_DataAll.m_DataMaps.m_PutSaveCode;
                std::wstring wstr2 = subDir.fullPath;
                // Удаление подстроки "C:\\Users\\Aio\\Documents\\Warcraft III\\CustomMapData" из пути
                size_t pos = wstr2.find(rootDirectory.fullPath);
                if (pos != std::wstring::npos) {
                    wstr2.erase(pos, rootDirectory.fullPath.length());
                }
                if (wstr2 == wstr) {
                    isPathSaveCodeFalse = false;
                    openDirectoryEntry = subDir;
                    isBackgroundDirectory = true;
                }
            }

            loadDirectories(fullPath, parent.subDirectories.back(), isPathSaveCodeFalse);
        }
    }
}

bool NewDirectory::isPushDirectory(const std::wstring& fullPath, const std::wstring& nameDirectory) const {
    const std::unordered_set<std::wstring> infoSkipDirectory = {L"Errors", L"unifont"};
    for (const auto& entry : std::filesystem::recursive_directory_iterator(fullPath)) {
        if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".txt" && !infoSkipDirectory.contains(nameDirectory))
            return true;
    }
    return false;
}

void NewDirectory::updateDirectoryTexts() {
    isBackgroundDirectory = false;
    directoryTexts.clear();
    directoryTriangles.clear(); // Очищаем треугольники

    float yOffset = 60.f + scrollOffset; // Применяем scrollOffset при расчете yOffset
    std::function<void(const DirectoryEntry&, float)> addTexts = [&](const DirectoryEntry& dir, float indent) {
        if (dir.isOpen) {
            for (const auto& subDir : dir.subDirectories) {
                    sf::Text dirText(subDir.name, font, 16);
                    dirText.setFillColor(sf::Color::Black);
                    dirText.setPosition(sf::Vector2f(38 + indent * 20, yOffset) + newPosition); // Смещение текста для размещения треугольника
                    directoryTexts.push_back(dirText);

                    if (!subDir.subDirectories.empty()) { // Проверяем, есть ли подкаталоги
                        sf::ConvexShape triangle(3); // Создаем треугольник
                        if (subDir.isOpen) {
                            triangle.setPoint(0, sf::Vector2f(15 + indent * 20, yOffset + 5) + newPosition);
                            triangle.setPoint(1, sf::Vector2f(25 + indent * 20, yOffset + 5) + newPosition);
                            triangle.setPoint(2, sf::Vector2f(20 + indent * 20, yOffset + 15) + newPosition);
                        }
                        else {
                            triangle.setPoint(0, sf::Vector2f(15 + indent * 20, yOffset + 5) + newPosition);
                            triangle.setPoint(1, sf::Vector2f(25 + indent * 20, yOffset + 10) + newPosition);
                            triangle.setPoint(2, sf::Vector2f(15 + indent * 20, yOffset + 15) + newPosition);
                        }
                        triangle.setFillColor(sf::Color::Black);
                        directoryTriangles.push_back(triangle); // Добавляем треугольник
                    }

                    if (isBackgroundDirectory == false && subDir.fullPath == openDirectoryEntry.fullPath && (yOffset + newPosition.y >= 60 && yOffset <= background.getSize().y - 60 + newPosition.y)) {
                        backgroundDirectory.setPosition(sf::Vector2f(0, yOffset) + newPosition);
                        isBackgroundDirectory = true;
                    }



                    yOffset += 30.f;
                    addTexts(subDir, indent + 1.f);
            }
        }
        };
    addTexts(rootDirectory, 0);
    adjustScrollbar();
}

void NewDirectory::adjustScrollbar() {
    float visibleArea = (background.getSize().y - 120) / 30;
    float thumbSize = scrollbar.getSize().y * (visibleArea / directoryTexts.size());
    thumbSize = (std::max)(thumbSize, 20.0f);
    scrollbarThumb.setSize(sf::Vector2f(10, thumbSize));

    // Пересчитываем позицию thumb после изменения его размера
    float scrollbarRange = scrollbar.getSize().y - scrollbarThumb.getSize().y;
    float scrollableArea = 30.0f * (directoryTexts.size() - visibleArea);
    float thumbPosition = (std::abs(scrollOffset) / scrollableArea) * scrollbarRange;
    thumbPosition = std::clamp(thumbPosition, 0.0f, scrollbarRange);
    scrollbarThumb.setPosition(sf::Vector2f(scrollbar.getPosition().x, scrollbar.getPosition().y + thumbPosition));
}

bool NewDirectory::newDirectory() {
    std::filesystem::create_directory(pathDatMapPut);

    // Открытие диалогового окна для выбора папки и получение пути к выбранной папке
    std::wstring folderPath = run();

    // Проверка, пуст ли путь
    if (folderPath == L"Exet") {
        std::wcerr << "folderPath1.empty()" << std::endl;
        return false;
    }

    // Удаление подстроки "C:\\Users\\Aio\\Documents\\Warcraft III\\CustomMapData" из пути
    size_t pos = folderPath.find(rootDirectory.fullPath);
    if (pos != std::wstring::npos) {
        folderPath.erase(pos, rootDirectory.fullPath.length());
    }

    // Проверка, пуст ли оставшийся путь
    if (folderPath.empty()) {
        std::wcerr << "folderPath2.empty()" << std::endl;
        return false;
    }

    // Создание полного пути
    std::wstring fullPath = pathDatMapPut + L'\\' + nameMaps;

    // Создание директорий по пути
    std::filesystem::create_directories(fullPath);

    // Добавление имени файла к пути
    fullPath += pathFile;

    std::ofstream outFile(fullPath, std::ios::binary);

    if (outFile) {
        StringConvector StringConvector_;
        std::string utf8Text = StringConvector_.utf16_to_utf8(folderPath);
        outFile.write(utf8Text.c_str(), utf8Text.size());

        outFile.close();
        std::wcout << "New path SaveCode ( " << pathDatMapPut + L'\\' + nameMaps << pathFile << L" )" << std::endl;
        return true; // Успешное выполнение
    }
    else {
        // Ошибка при создании файла
        std::wcerr << "Failed to create file: " << pathDatMapPut << L'\\' << nameMaps << pathFile << L'\n';
        return false;
    }
    return false;
}

void NewDirectory::drawWindow() {
    window.clear(sf::Color(0, 255, 0));
    window.draw(background);

    if (numMenu == 1) {

        for (const auto& dirText : directoryTexts) { // Сначала рисуем тексты
            if (dirText.getPosition().y >= topLine.getPosition().y - 30 && dirText.getPosition().y <= bottomLine.getPosition().y + 10) {
                window.draw(dirText);
            }
        }

        for (const auto& triangle : directoryTriangles) { // Затем рисуем треугольники
            // Проверяем координаты Y всех точек треугольника
            float triangleY = (triangle.getPoint(0).y + triangle.getPoint(1).y + triangle.getPoint(2).y) / 3; // Средняя Y координата
            if (triangleY >= topLine.getPosition().y - 30 && triangleY <= bottomLine.getPosition().y + 10) {
                window.draw(triangle);
            }
        }

        if (isBackgroundDirectory)
            if (backgroundDirectory.getPosition().y >= topLine.getPosition().y - 30 && backgroundDirectory.getPosition().y <= bottomLine.getPosition().y + 10)
                window.draw(backgroundDirectory);


        //window.draw(scrollbar);
        window.draw(scrollbarThumb);

    }
    else if (numMenu == 2) {
        for (sf::Text& p : m_TextArray)
            window.draw(p);
        window.draw(bottomLineCommandsData);

        for (CommandsData& p : m_CommandsData)
            p.drawCommands(window);
    }
    else if (numMenu == 3) {
        for (RowData& p : m_RowData)
            p.drawRow(window);
    }


    window.draw(topBackground);
    window.draw(bottomBackground);

    window.draw(topLine);
    window.draw(bottomLine);

    window.draw(title);

    window.draw(directoryButton);
    window.draw(directoryText);
    window.draw(commandsButton);
    window.draw(commandsText);
    window.draw(optionsButton);
    window.draw(optionsText);

    window.draw(closeButton);
    window.draw(closeText);

    if (numMenu == 1) {
        window.draw(selectedTextNo);
        window.draw(selectedTextYes);
    }

    window.display();
}

void NewDirectory::toggleDirectory(DirectoryEntry& directory) {
    directory.isOpen = !directory.isOpen;
    updateDirectoryTexts();
}

std::wstring NewDirectory::newPath() {
    std::wstring ownerPath = L"\0";
    ownerPath = openDirectoryEntry.fullPath;

    if (openDirectoryEntry.subDirectories.size() == 1) {
        bool isWhile = true;
        while (isWhile) {
            for (const auto& entry : std::filesystem::directory_iterator(ownerPath)) {
                if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".txt") {
                    isWhile = false;
                    break;
                }
                else if (std::filesystem::is_directory(entry)) {
                    for (const auto& entry : std::filesystem::directory_iterator(entry)) {
                        if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".txt") {
                            isWhile = false;
                            break;
                        }
                    }
                }

            }
            if (isWhile) {
                openDirectoryEntry = openDirectoryEntry.subDirectories[0];
                ownerPath = openDirectoryEntry.fullPath;
            }
        }
    }
    return ownerPath;
}

std::wstring NewDirectory::handleMousePress(sf::Event& event) {
    if (event.mouseButton.button != sf::Mouse::Left)
        return L"";

    const sf::Vector2f mouseButton{ static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };

    // Определяем действия для каждой кнопки меню
    std::map<sf::RectangleShape*, std::function<void()>> menuActions{
        {&directoryButton, [&]() {
            if (numMenu != 1) {
                numMenu = 1;
                activeMenu();
                title.setString(L"Maps: " + nameMaps);
            }
        }},
        {&commandsButton, [&]() {
            if (numMenu != 2) {
                numMenu = 2;
                activeMenu();
                title.setString(L"Maps: " + nameMaps);
            }
        }},
        {&optionsButton, [&]() {
            if (numMenu != 3) {
                numMenu = 3;
                activeMenu();
                title.setString("All Maps");
            }
        }}
    };

    if (closeButton.getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
        return L"Exet";
    }

    // Проверяем, нажата ли одна из кнопок меню
    for (const auto& [button, action] : menuActions) {
        if (button->getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
            action();
            return L"";
        }
    }

    if (numMenu == 1) {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        if (scrollbarThumb.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
            isScrolling = true;
            mouseOffset = mousePosition.y - scrollbarThumb.getPosition().y;
            return L"";
        }

        if (mouseButton.y >= bottomLine.getPosition().y && mouseButton.y <= background.getSize().y + newPosition.y) {
            if (openDirectoryEntry.fullPath.empty() || mouseButton.x >= 0 + newPosition.x && mouseButton.x <= background.getSize().x / 3 + newPosition.x)
                return L"False";
            else {
                return newPath();
            }
        }

        if (mouseButton.y < topLine.getPosition().y || mouseButton.y > bottomLine.getPosition().y)
            return L"";

        float yOffset = 60.f + scrollOffset;

        std::function<bool(DirectoryEntry&, int)> processClick = [&](DirectoryEntry& dir, int indent) {
            if (dir.isOpen) {
                const sf::Vector2f mouseButtin{ mouseButton.x, mouseButton.y };
                for (auto& subDir : dir.subDirectories) {
                    sf::FloatRect textBounds(newPosition.x, yOffset + newPosition.y, background.getSize().x - 60, 30);

                    if (!subDir.subDirectories.empty()) {
                        sf::FloatRect triangleBounds(10 + indent * 20 + newPosition.x, yOffset + newPosition.y, 25, 25);
                        if (triangleBounds.contains(mouseButtin.x, mouseButtin.y)) {
                            toggleDirectory(subDir);
                            return true;
                        }
                    }
                    if (mouseButtin.y <= bottomLine.getPosition().y && textBounds.contains(mouseButtin.x, mouseButtin.y)) {
                        openDirectoryEntry = subDir;
                        subDir.isOpen = true;
                        updateDirectoryTexts();
                        return true;
                    }

                    yOffset += 30;
                    if (processClick(subDir, indent + 1)) return true;
                }
            }
            return false;
            };

        processClick(rootDirectory, 0);
    }
    else if (numMenu == 2) {
        short isSquare = 0;
        int numCommands = -1;
        for (int i = 0; i < m_CommandsData.size(); ++i) {
            if (m_CommandsData[i].square.getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
                numCommands = i;
                isSquare = 1;
                break;
            }
            else if (m_CommandsData[i].square2.getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
                numCommands = i;
                isSquare = 2;
                break;
            }
        }

        if (isSquare != 0) {
            auto& cmdData = m_CommandsData[numCommands];
            bool& value = (isSquare == 1) ? cmdData.value1 : cmdData.value2;
            value = !value;

            std::vector<std::function<void()>> commands = {
                [&] { m_DataAll.m_Commands.returnCmd.isVisibleButton = !m_DataAll.m_Commands.returnCmd.isVisibleButton; },
                [&] { m_DataAll.m_Commands.saveCmd.isVisibleButton = !m_DataAll.m_Commands.saveCmd.isVisibleButton; },
                [&] { m_DataAll.m_Commands.craftCmd.isVisibleButton = !m_DataAll.m_Commands.craftCmd.isVisibleButton; },
                [&] { m_DataAll.m_Commands.camCmd.isVisibleButton = !m_DataAll.m_Commands.camCmd.isVisibleButton; },
                [&] { m_DataAll.m_Commands.statsCmd.isVisibleButton = !m_DataAll.m_Commands.statsCmd.isVisibleButton; },
                [&] { m_DataAll.m_Commands.clearCmd.isVisibleButton = !m_DataAll.m_Commands.clearCmd.isVisibleButton; },
                [&] { m_DataAll.m_Commands.cleanCmd.isVisibleButton = !m_DataAll.m_Commands.cleanCmd.isVisibleButton; },
                [&] { m_DataAll.m_Commands.strCmd.isVisibleButton = !m_DataAll.m_Commands.strCmd.isVisibleButton; },
                [&] { m_DataAll.m_Commands.agiCmd.isVisibleButton = !m_DataAll.m_Commands.agiCmd.isVisibleButton; },
                [&] { m_DataAll.m_Commands.intCmd.isVisibleButton = !m_DataAll.m_Commands.intCmd.isVisibleButton; }
            };

            std::vector<std::function<void()>> loadCommands = {
                [&] { m_DataAll.m_Commands.returnCmd.isLoad = !m_DataAll.m_Commands.returnCmd.isLoad; },
                [&] { m_DataAll.m_Commands.saveCmd.isLoad = !m_DataAll.m_Commands.saveCmd.isLoad; },
                [&] { m_DataAll.m_Commands.craftCmd.isLoad = !m_DataAll.m_Commands.craftCmd.isLoad; },
                [&] { m_DataAll.m_Commands.camCmd.isLoad = !m_DataAll.m_Commands.camCmd.isLoad; },
                [&] { m_DataAll.m_Commands.statsCmd.isLoad = !m_DataAll.m_Commands.statsCmd.isLoad; },
                [&] { m_DataAll.m_Commands.clearCmd.isLoad = !m_DataAll.m_Commands.clearCmd.isLoad; },
                [&] { m_DataAll.m_Commands.cleanCmd.isLoad = !m_DataAll.m_Commands.cleanCmd.isLoad; },
                [&] { m_DataAll.m_Commands.strCmd.isLoad = !m_DataAll.m_Commands.strCmd.isLoad; },
                [&] { m_DataAll.m_Commands.agiCmd.isLoad = !m_DataAll.m_Commands.agiCmd.isLoad; },
                [&] { m_DataAll.m_Commands.intCmd.isLoad = !m_DataAll.m_Commands.intCmd.isLoad; }
            };

            if (isSquare == 1) {
                commands[numCommands]();
            }
            else {
                loadCommands[numCommands]();
            }

            cmdData.updateCheckText();
            LoadCommands LoadCommands_(m_DataAll);
            LoadCommands_.saveCommands();
        }
    }
    else if (numMenu == 3) {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        auto checkAndToggle = [&](int index, bool& option, bool& isSave) {
            if (m_RowData[index].square.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                m_RowData[index].value = !m_RowData[index].value;
                m_RowData[index].updateCheckText();
                option = !option;
                isSave = true;
            }
            };

        bool isSave = false;
        checkAndToggle(0, m_DataAll.m_OptionsData.autoСlickerKey, isSave);
        checkAndToggle(1, m_DataAll.m_OptionsData.autoСlickerMaus, isSave);

        if (isSave) {
            m_DataAll.m_OptionsData.saveFileOptions();
        }
    }

    return L"";
}

void NewDirectory::handleScroll(sf::Event& event) {
    float delta = event.mouseWheelScroll.delta;
    scrollOffset += delta * scrollSpeed * 30;
    scrollOffset = std::clamp(scrollOffset, -30.0f * (directoryTexts.size() - (background.getSize().y - 120) / 30), 0.0f);
    updateDirectoryTexts();
}

void NewDirectory::handleMouseMove(sf::Event& event) {
    if (isScrolling) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        float scrollbarRange = scrollbar.getSize().y - scrollbarThumb.getSize().y;
        float scrollableArea = 30.0f * directoryTexts.size() - (background.getSize().y - 120);

        // Используем сохраненное смещение при расчете позиции
        float thumbPosition = mousePosition.y - scrollbar.getPosition().y - mouseOffset;
        thumbPosition = std::clamp(thumbPosition, 0.0f, scrollbarRange);
        scrollbarThumb.setPosition(sf::Vector2f(scrollbar.getPosition().x, scrollbar.getPosition().y + thumbPosition) + newPosition);

        scrollOffset = -(thumbPosition / scrollbarRange) * scrollableArea;
        updateDirectoryTexts();
    }
}

bool NewDirectory::IsWindowInFocus(const HWND& hWnd) const {
    if (hWnd == NULL) {
        //std::wcout << L"Нет активного окна!" << std::endl;
        return 0;
    }

    // Буфер для имени окна
    wchar_t windowTitle[256];

    // Получаем имя окна
    int length = GetWindowText(hWnd, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
    if (length == 0) {
        //std::wcout << L"Не удалось получить имя окна!" << std::endl;
        return 0;
    }

    // Сравниваем имя окна с ожидаемым
    if (wcscmp(windowTitle, L"Warcraft III") != 0) {
        //std::wcout << L"Имя активного окна не совпадает с 'Warcraft III'" << windowTitle << std::endl;
        return 0;
    }

    //std::wcout << windowTitle << std::endl;


    return true;
}

std::wstring NewDirectory::run() {
    bool isActive = true;
    while (window.isOpen()) {

        // Получаем дескриптор активного окна
        HWND hWndWindow = GetForegroundWindow();

        //HWND hWndWindow = FindWindow(NULL, L"Warcraft III");
        if (IsWindowInFocus(hWndWindow)) {

            if (isActive == false) {
                if (m_DataAll.m_DataPath.m_hWndWindow != hWndWindow)
                    return std::wstring();
                isActive = true;
            }

            sf::Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case sf::Event::MouseButtonPressed:
                {
                    std::wstring wstr = handleMousePress(event);
                    if (wstr != L"\0")
                        return wstr;
                }
                break;
                case sf::Event::MouseWheelScrolled:
                    handleScroll(event);
                    break;
                case sf::Event::MouseMoved:
                    handleMouseMove(event);
                    break;
                case sf::Event::MouseButtonReleased:
                    isScrolling = false;
                    break;
                default:
                    break;
                }
            }
            drawWindow();
        }
        else {
            isActive = false;
            sf::Event event;
            while (window.pollEvent(event)) {

            }
            window.clear(sf::Color(0, 255, 0));
            window.display();
        }
        if (isExetTree)
            window.close();
    }
    return std::wstring();
}
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
#include "LoadComands.h"
#include "StringConvector.h"

#include "NewDirectory.h"
#include <unordered_set>

NewDirectory::NewDirectory(sf::RenderWindow& t_Window, sf::Font& t_font, const sf::Vector2f& t_newPosition, const std::wstring& t_nameMaps, const std::wstring& t_PathWar, bool& t_isExetTree, OptionsData& t_OptionsData) :
    window(t_Window),
    font(t_font),
    newPosition(t_newPosition),
    nameMaps(t_nameMaps),
    rootDirectory(L"CustomMapData", t_PathWar),
    isExetTree(t_isExetTree),
    scrollOffset(0),
    isScrolling(false),
    scrollSpeed(1.0f),
    openDirectoryEntry(rootDirectory),
    m_OptionsData(t_OptionsData)
{
    initializeWindow();
}

void NewDirectory::initializeText(sf::Text& text, const std::wstring& textString, const sf::Vector2f& position, const sf::Color& color) {
    text.setFont(font);
    text.setCharacterSize(16);

    text.setString(textString);
    text.setPosition(position);
    text.setFillColor(color);
}

void NewDirectory::initializeRectangle(sf::RectangleShape& rectangle, const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color) {
    rectangle.setSize(size);
    rectangle.setPosition(position);
    rectangle.setFillColor(color);
}


void NewDirectory::initializeWindow() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    float windowWidth = 500;
    float windowHeight = desktop.height * 2.f / 3.f;

    if(newPosition == sf::Vector2f(0, 0))
        newPosition = sf::Vector2f(static_cast<float>(desktop.width) / 2.f - windowWidth/2.f,
            static_cast<float>(desktop.height) / 2.f - windowHeight / 2.f);

    //
    //  общяя часть меню
    //
    initializeRectangle(background,
        sf::Vector2f(windowWidth, windowHeight),
        newPosition
        );

    initializeText(title,
        L"Maps: " + nameMaps, 
        newPosition + sf::Vector2f{20, 0});

    initializeRectangle(topLine,
        sf::Vector2f(windowWidth, 2),
        sf::Vector2f(0, 55) + newPosition,
        sf::Color::Black
    );

    initializeRectangle(bottomLine,
        sf::Vector2f(windowWidth, 2),
        sf::Vector2f(0, windowHeight - 35) + newPosition,
        sf::Color::Black
    );

    initializeRectangle(closeButton,
        sf::Vector2f(20, 19),
        sf::Vector2f(windowWidth - 30, -5) + newPosition,
        sf::Color::Red
    );

    initializeText(closeText, 
        L"X", 
        newPosition + sf::Vector2f{ windowWidth - 25, -5 },
        sf::Color::White);

    initializeText(selectedTextNo,
        L"Пропустить", 
        newPosition + sf::Vector2f{ 40, windowHeight - 28 });

    initializeText(selectedTextYes,
        L"Подтвердить выбор", 
        newPosition + sf::Vector2f{ windowWidth / 2, windowHeight - 28 });

    initializeRectangle(topBackground,
        sf::Vector2f(windowWidth, topLine.getPosition().y - newPosition.y + 3),
        sf::Vector2f(0, -15) + newPosition
    );

    initializeRectangle(bottomBackground,
        sf::Vector2f(windowWidth, 35),
        sf::Vector2f(newPosition.x, bottomLine.getPosition().y - 1)
    );

    const std::vector<std::pair<std::wstring, std::pair<sf::RectangleShape&, sf::Text&>>> nameRazels = {
        {L"Directory", {directoryButton, directoryText}},
        {L"Comands", {commandsButton, commandsText}},
        {L"Options", {optionsButton, optionsText}},
    };
    const int maxSize = nameRazels.size();

    for (int i = 0; const std::pair<std::wstring, std::pair<sf::RectangleShape&, sf::Text&>>& p : nameRazels) {
        initializeRectangle(p.second.first,
            sf::Vector2f(windowWidth / maxSize, 30),
            sf::Vector2f(newPosition.x + windowWidth / maxSize * i, newPosition.y + 25)
        );
        initializeText(p.second.second, p.first, (p.second.first.getPosition() + sf::Vector2f{ 20, 5 }));
        i++;
    }

    activeMenu();

    //
    //  меню выбора дириктории
    //
    scrollbar.setSize(sf::Vector2f(10, windowHeight - 93));
    //scrollbar.setFillColor(sf::Color(200, 200, 200));
    //scrollbar.setOutlineThickness(1);
    //scrollbar.setOutlineColor(sf::Color::Black);
    scrollbar.setPosition(sf::Vector2f(windowWidth - 10, 58) + newPosition);

    scrollbarThumb.setFillColor(sf::Color(100, 100, 100));
    scrollbarThumb.setPosition(scrollbar.getPosition());

    backgroundDirectory.setFillColor(sf::Color(0, 60, 160, 80));
    backgroundDirectory.setSize(sf::Vector2f(windowWidth - 15, 25));

    rootDirectory.isOpen = true;
    loadDirectories(rootDirectory.fullPath, rootDirectory);
    updateDirectoryTexts();


    //
    //  меню настройки команд
    //

    m_TextArray.resize(3);
    const std::vector<std::pair<std::string, float>> dat{
        {"Кнопка", 10},
        {"После загрузки", 100},
        {"команда", 280},
    };
    initializeTextArray(dat, 14);

    if (!m_DataPath.initializePaths()) {
        return;
    }

    LoadComands LoadComands_(m_DataPath);
    m_Comands = LoadComands_.loadComands();

    m_ComandsData.resize(4);
    m_ComandsData[0].initialize(m_Comands.returnCmd.isVisibleButton, m_Comands.returnCmd.isLoad, font, m_Comands.returnCmd.str, 20, sf::Color(180, 180, 180), sf::Color::Black);
    m_ComandsData[0].setPosition(newPosition.x + 25, newPosition.y + 85);

    m_ComandsData[1].initialize(m_Comands.saveCmd.isVisibleButton, m_Comands.saveCmd.isLoad, font, m_Comands.saveCmd.str, 20, sf::Color(180, 180, 180), sf::Color::Black);
    m_ComandsData[1].setPosition(newPosition.x + 25, newPosition.y + 110);

    m_ComandsData[2].initialize(m_Comands.craftCmd.isVisibleButton, m_Comands.craftCmd.isLoad, font, m_Comands.craftCmd.str, 20, sf::Color(180, 180, 180), sf::Color::Black);
    m_ComandsData[2].setPosition(newPosition.x + 25, newPosition.y + 135);

    m_ComandsData[3].initialize(m_Comands.camCmd.isVisibleButton, m_Comands.camCmd.isLoad, font, m_Comands.camCmd.str, 20, sf::Color(180, 180, 180), sf::Color::Black);
    m_ComandsData[3].setPosition(newPosition.x + 25, newPosition.y + 160);

    //m_ComandsData[4].initialize(m_Comands.statsCmd.isVisibleButton, m_Comands.statsCmd.isLoad, font, m_Comands.statsCmd.str, 20, sf::Color(180, 180, 180), sf::Color::Black);
    //m_ComandsData[4].setPosition(newPosition.x + 25, newPosition.y + 185);

    //m_ComandsData[5].initialize(m_Comands.clearCmd.isVisibleButton, m_Comands.clearCmd.isLoad, font, m_Comands.clearCmd.str, 20, sf::Color(180, 180, 180), sf::Color::Black);
    //m_ComandsData[5].setPosition(newPosition.x + 25, newPosition.y + 210);

    //m_ComandsData[6].initialize(m_Comands.cleanCmd.isVisibleButton, m_Comands.cleanCmd.isLoad, font, m_Comands.cleanCmd.str, 20, sf::Color(180, 180, 180), sf::Color::Black);
    //m_ComandsData[6].setPosition(newPosition.x + 25, newPosition.y + 235);

    //m_ComandsData[7].initialize(m_Comands.strCmd.isVisibleButton, m_Comands.strCmd.isLoad, font, m_Comands.strCmd.str, 20, sf::Color(180, 180, 180), sf::Color::Black);
    //m_ComandsData[7].setPosition(newPosition.x + 25, newPosition.y + 260);

    //m_ComandsData[8].initialize(m_Comands.agiCmd.isVisibleButton, m_Comands.agiCmd.isLoad, font, m_Comands.agiCmd.str, 20, sf::Color(180, 180, 180), sf::Color::Black);
    //m_ComandsData[8].setPosition(newPosition.x + 25, newPosition.y + 285);

    //m_ComandsData[9].initialize(m_Comands.intCmd.isVisibleButton, m_Comands.intCmd.isLoad, font, m_Comands.intCmd.str, 20, sf::Color(180, 180, 180), sf::Color::Black);
    //m_ComandsData[9].setPosition(newPosition.x + 25, newPosition.y + 310);

    //
    //  меню настрояк 
    //

    m_RowData.resize(2);

    m_RowData[0].initialize(m_OptionsData.autoСlickerKey, font, "Автонажатия клавишь", 24, sf::Color(180, 180, 180), sf::Color::Black);
    m_RowData[0].setPosition(newPosition.x + 20, newPosition.y + 70);

    m_RowData[1].initialize(m_OptionsData.autoСlickerMaus, font, "Автонажатия правой кнопки мыши", 24, sf::Color(180, 180, 180), sf::Color::Black);
    m_RowData[1].setPosition(newPosition.x + 20, newPosition.y + 100);

    //m_RowData[2].initialize(m_OptionsData.autoStart, font, "Автоматический запуск программы", 24, sf::Color(180, 180, 180), sf::Color::Black);
    //m_RowData[2].setPosition(newPosition.x + 20, newPosition.y + 130);


    window.setFramerateLimit(60);
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
}

void NewDirectory::activeMenu() {
    if (numMenu == 1) {
        directoryButton.setFillColor(sf::Color(100, 100, 250)); // Active color
        directoryText.setFillColor(sf::Color::White);

        commandsButton.setFillColor(sf::Color(200, 200, 200)); // Inactive color
        commandsText.setFillColor(sf::Color::Black);

        optionsButton.setFillColor(sf::Color(200, 200, 200)); // Inactive color
        optionsText.setFillColor(sf::Color::Black);

    }
    if (numMenu == 2) {
        commandsButton.setFillColor(sf::Color(100, 100, 250)); // Active color
        commandsText.setFillColor(sf::Color::White);

        directoryButton.setFillColor(sf::Color(200, 200, 200)); // Inactive color
        directoryText.setFillColor(sf::Color::Black);

        optionsButton.setFillColor(sf::Color(200, 200, 200)); // Inactive color
        optionsText.setFillColor(sf::Color::Black);
    }
    if (numMenu == 3) {
        optionsButton.setFillColor(sf::Color(100, 100, 250)); // Active color
        optionsText.setFillColor(sf::Color::White);

        commandsButton.setFillColor(sf::Color(200, 200, 200)); // Inactive color
        commandsText.setFillColor(sf::Color::Black);

        directoryButton.setFillColor(sf::Color(200, 200, 200)); // Inactive color
        directoryText.setFillColor(sf::Color::Black);
    }
}

void NewDirectory::loadDirectories(const std::wstring& directoryPath, DirectoryEntry& parent) {
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
            loadDirectories(fullPath, parent.subDirectories.back());
        }
    }
}

bool NewDirectory::isPushDirectory(const std::wstring& fullPath, const std::wstring& nameDirectory) {
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

        for (ComandsData& p : m_ComandsData)
            p.drawComands(window);
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

std::wstring NewDirectory::newPath(){
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

    if (directoryButton.getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
        if (numMenu == 1)
            return L"";
        
        numMenu = 1;
        activeMenu();
        title.setString(L"Maps: " + nameMaps);

        return L"";
    }
    else if (commandsButton.getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
        if (numMenu == 2)
            return L"";

        numMenu = 2;
        activeMenu();
        title.setString(L"Maps: " + nameMaps);

        return L"";
    }
    else if (optionsButton.getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
        if (numMenu == 3)
            return L"";

        numMenu = 3;
        activeMenu();
        title.setString("All Maps");

        return L"";
    }

    if (closeButton.getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
        return L"Exet";
    }

    if (numMenu == 1) {
        {
            const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

            if (scrollbarThumb.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                isScrolling = true;
                mouseOffset = mousePosition.y - scrollbarThumb.getPosition().y; // Сохранение смещения
                return L"";
            }
        }



        if (mouseButton.y >= bottomLine.getPosition().y && mouseButton.y <= background.getSize().y + newPosition.y) {
            if (openDirectoryEntry.fullPath.empty() || mouseButton.x >= 0 + newPosition.x && mouseButton.x <= background.getSize().x / 3 + newPosition.x)
                return L"False";
            else {
                return newPath();
            }
        }

        if ((mouseButton.y < topLine.getPosition().y) || (mouseButton.y > bottomLine.getPosition().y))
            return L"";

        float yOffset = 60.f + scrollOffset; // Применяем scrollOffset при расчете yOffset

        std::function<bool(DirectoryEntry&, int)> processClick = [&](DirectoryEntry& dir, int indent) {
            if (dir.isOpen) {
                const sf::Vector2f mouseButtin{ mouseButton.x, mouseButton.y };
                for (auto& subDir : dir.subDirectories) {
                    sf::FloatRect textBounds(/*35 + indent * 20 + */newPosition.x, yOffset + newPosition.y, background.getSize().x - 60, 30);

                    // Проверяем, есть ли подкаталоги, и создаем треугольник, если они есть
                    if (!subDir.subDirectories.empty()) {
                        sf::FloatRect triangleBounds(10 + indent * 20 + newPosition.x, yOffset - 0 + newPosition.y, 25, 25);
                        if (triangleBounds.contains(mouseButtin.x, mouseButtin.y)) {
                            toggleDirectory(subDir);
                            return true;
                        }
                    }
                    if (mouseButtin.y <= bottomLine.getPosition().y && textBounds.contains(mouseButtin.x, mouseButtin.y)) {
                        openDirectoryEntry = subDir;
                        subDir.isOpen = true; // Automatically open subdirectories
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
        int numComands = -1;
        short isSquare = 0;
        for (int i = 0; i < m_ComandsData.size(); ++i) {
            if (m_ComandsData[i].square.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                numComands = i;
                isSquare = 1;
                break;
            }
        }

        if(isSquare == 0)
            for (int i = 0; i < m_ComandsData.size(); ++i) {
                if (m_ComandsData[i].square2.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    numComands = i;
                    isSquare = 2;
                    break;
                }
            }

        if (isSquare == 0)
            return L"";
        else if (isSquare == 1) {
            m_ComandsData[numComands].value1 = !m_ComandsData[numComands].value1;

            switch ((numComands+1))
            {
            case 1:
                m_Comands.returnCmd.isVisibleButton = !m_Comands.returnCmd.isVisibleButton;
                break;
            case 2:
                m_Comands.saveCmd.isVisibleButton = !m_Comands.saveCmd.isVisibleButton;
                break;
            case 3:
                m_Comands.craftCmd.isVisibleButton = !m_Comands.craftCmd.isVisibleButton;
                break;
            case 4:
                m_Comands.camCmd.isVisibleButton = !m_Comands.camCmd.isVisibleButton;
                break;
            case 5:
                m_Comands.statsCmd.isVisibleButton = !m_Comands.statsCmd.isVisibleButton;
                break;
            case 6:
                m_Comands.clearCmd.isVisibleButton = !m_Comands.clearCmd.isVisibleButton;
                break;
            case 7:
                m_Comands.cleanCmd.isVisibleButton = !m_Comands.cleanCmd.isVisibleButton;
                break;
            case 8:
                m_Comands.strCmd.isVisibleButton = !m_Comands.strCmd.isVisibleButton;
                break;
            case 9:
                m_Comands.agiCmd.isVisibleButton = !m_Comands.agiCmd.isVisibleButton;
                break;
            case 10:
                m_Comands.intCmd.isVisibleButton = !m_Comands.intCmd.isVisibleButton;
                break;
            default:
                break;
            }
        }
        else if (isSquare == 2) {
            m_ComandsData[numComands].value2 = !m_ComandsData[numComands].value2;

            switch ((numComands + 1))
            {
            case 1:
                m_Comands.returnCmd.isLoad = !m_Comands.returnCmd.isLoad;
                break;
            case 2:
                m_Comands.saveCmd.isLoad = !m_Comands.saveCmd.isLoad;
                break;
            case 3:
                m_Comands.craftCmd.isLoad = !m_Comands.craftCmd.isLoad;
                break;
            case 4:
                m_Comands.camCmd.isLoad = !m_Comands.camCmd.isLoad;
                break;
            case 5:
                m_Comands.statsCmd.isLoad = !m_Comands.statsCmd.isLoad;
                break;
            case 6:
                m_Comands.clearCmd.isLoad = !m_Comands.clearCmd.isLoad;
                break;
            case 7:
                m_Comands.cleanCmd.isLoad = !m_Comands.cleanCmd.isLoad;
                break;
            case 8:
                m_Comands.strCmd.isLoad = !m_Comands.strCmd.isLoad;
                break;
            case 9:
                m_Comands.agiCmd.isLoad = !m_Comands.agiCmd.isLoad;
                break;
            case 10:
                m_Comands.intCmd.isLoad = !m_Comands.intCmd.isLoad;
                break;
            default:
                break;
            }
        }

        m_ComandsData[numComands].updateCheckText();

        LoadComands LoadComands_(m_DataPath);
        LoadComands_.saveComands(m_Comands);

    }
    else  if (numMenu == 3) {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        bool isSave = false;
        if (m_RowData[0].square.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
            m_RowData[0].value = !m_RowData[0].value;
            m_RowData[0].updateCheckText();
            m_OptionsData.autoСlickerKey = !m_OptionsData.autoСlickerKey;
            isSave = true;
        }
        else if (m_RowData[1].square.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
            m_RowData[1].value = !m_RowData[1].value;
            m_RowData[1].updateCheckText();
            m_OptionsData.autoСlickerMaus = !m_OptionsData.autoСlickerMaus;
            isSave = true;
        }
        /*else if (m_RowData[2].square.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
            m_RowData[2].value = !m_RowData[2].value;
            m_RowData[2].updateCheckText();
            m_OptionsData.autoStart = !m_OptionsData.autoStart;
            isSave = true;

            avtostart(m_RowData[2].value);

        }*/
        if (isSave) {
            m_OptionsData.saveFileOptions();
            isSave = false;
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

bool NewDirectory::IsWindowInFocus(HWND hWnd) {
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
    while (window.isOpen()) {

        // Получаем дескриптор активного окна
        HWND hWndWindow = GetForegroundWindow();

        //HWND hWndWindow = FindWindow(NULL, L"Warcraft III");

        if (IsWindowInFocus(hWndWindow)) {
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
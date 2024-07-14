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
#include "LoadDataCommands.h"
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

inline void NewDirectory::initializeText(sf::Text& text, const std::wstring& textString, const sf::Vector2f& position, const sf::Color& color, const float& letterSpacing) const {
    text.setFont(font);
    text.setCharacterSize(16);
    text.setLetterSpacing(letterSpacing);;

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

    // ����� ����
    initializeRectangle(background, sf::Vector2f(windowWidth, windowHeight), newPosition);
    initializeText(title, L"Maps: " + m_DataAll.m_DataMaps.m_NameMapsFull, newPosition + sf::Vector2f{ 20, 0 });
    initializeLine(topLine, sf::Vector2f(windowWidth, 2), sf::Vector2f(0, 55) + newPosition);
    initializeLine(bottomLine, sf::Vector2f(windowWidth, 2), sf::Vector2f(0, windowHeight - 35) + newPosition);
    initializeRectangle(closeButton, sf::Vector2f(20, 19), sf::Vector2f(windowWidth - 30, -5) + newPosition, sf::Color::Red);
    initializeText(closeText, L"X", newPosition + sf::Vector2f{ windowWidth - 25, -5 }, sf::Color::White);
    initializeText(selectedTextNo, L"����������", newPosition + sf::Vector2f{ 40, windowHeight - 28 });
    initializeText(selectedTextYes, L"����������� �����", newPosition + sf::Vector2f{ windowWidth / 2, windowHeight - 28 });
    initializeRectangle(topBackground, sf::Vector2f(windowWidth, topLine.getPosition().y - newPosition.y + 3), sf::Vector2f(0, -15) + newPosition);
    initializeRectangle(bottomBackground, sf::Vector2f(windowWidth, 35), sf::Vector2f(newPosition.x, bottomLine.getPosition().y - 1));

    const std::vector<std::pair<std::wstring, std::pair<sf::RectangleShape&, sf::Text&>>> nameRazels = {
        {L"Directory", {directoryButton, directoryText}},
        {L"DataCommands", {DataCommandsButton, DataCommandsText}},
        {L"Options", {optionsButton, optionsText}},
    };
    const int maxSize = nameRazels.size();

    for (int i = 0; i < maxSize; ++i) {
        const auto& [name, components] = nameRazels[i];
        auto& [button, text] = components;
        initializeRectangle(button, sf::Vector2f(windowWidth / maxSize, 30), newPosition + sf::Vector2f(windowWidth / maxSize * i, 25));
        initializeText(text, name, button.getPosition() + sf::Vector2f{ 20, 5 }, sf::Color::Black, 1.5f);
    }

    activeMenu();

    // ���� ������ ����������
    initializeScrollbar(windowWidth, windowHeight);

    bool isPathSaveCodeFalse = false;
    const std::wstring& wstr = m_DataAll.m_DataMaps.m_PutSaveCode;
    if (wstr != L"False" && !wstr.empty()) {
        isPathSaveCodeFalse = true;
    }
    loadDirectories(rootDirectory.fullPath, rootDirectory, isPathSaveCodeFalse);

    updateDirectoryTexts();

    // ���� ��������� ������
    initializeDataCommands();

    // ���� ��������
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
    backgroundDirectory.setSize(sf::Vector2f(windowWidth, 30));

    rootDirectory.isOpen = true;
}


void NewDirectory::initializeDataCommands() {
    const float OFFSET_X = 25;
    const float INITIAL_Y = 81;

    std::vector<std::string> commandNames = {
        "[Return1]", "[Return2]", "[Return3]", "[Save]", "[Craft]", "[Cam]", "[Stats]", "[Clear]", "[Clean]", "[Str]", "[Agi]", "[Int]"
    };

    m_CommandsUI_1.resize(commandNames.size());

    const sf::Color buttonColor(180, 180, 180);
    const sf::Color buttonTextColor = sf::Color::Black;

    for (size_t i = 0; i < commandNames.size(); ++i) {
        const std::string& commandName = commandNames[i];
        auto& command = *m_DataAll.m_DataCommands.commandMap[commandName];

        const bool isCamCommand = (commandName == "[Cam]" || commandName == "[Return1]" || commandName == "[Return2]" || commandName == "[Return3]");
        m_CommandsUI_1[i].initialize(command.isVisibleButton, command.isLoad, font, command.str, 18, buttonColor, buttonTextColor, isCamCommand, command.hotKey);
        
        float positionY = INITIAL_Y + (i * OFFSET_X);
        m_CommandsUI_1[i].setPosition(newPosition.x, newPosition.y + positionY, background.getSize().x, OFFSET_X);
    }

    m_TextArray.resize(3);
    const std::vector<std::pair<std::string, float>> dat{
        {"DataCommands", 10},
        {"Visible", background.getSize().x/2},
        {"AVDataCommands", background.getSize().x / 2 + 52},
    };
    initializeTextArray(dat, 12);
}

void NewDirectory::initializeSettings() {
    const std::vector<std::tuple<std::string, sf::Vector2f, bool>> settings = {
        {"����������� �������", {20, 70}, m_DataAll.m_OptionsData.auto�lickerKey},
        {"����������� ������ ������ ����", {20, 100}, m_DataAll.m_OptionsData.auto�lickerMaus},
        {"��������������", {20, 130}, m_DataAll.m_OptionsData.autoSave},
        {"���� ������ (����������)", {20, 160}, m_DataAll.m_OptionsData.autoStart},
        {"���� �������� ������������", {20, 190}, m_DataAll.m_OptionsData.autoSkillsUpgrade},
    };

    m_OptionsUI_1.resize(settings.size());

    for (size_t i = 0; i < settings.size(); ++i) {
        const auto& [label, positionOffset, optionData] = settings[i];
        m_OptionsUI_1[i].initialize(optionData, font, label, 18, sf::Color(180, 180, 180), sf::Color::Black);
        m_OptionsUI_1[i].setPosition(newPosition.x + positionOffset.x, newPosition.y + positionOffset.y);
    }
}

void NewDirectory::initializeTextArray(const std::vector<std::pair<std::string, float>>& dat, unsigned int characterSize)
{
    for (size_t i = 0; i < m_TextArray.size(); ++i)
    {
        m_TextArray[i].setFont(font);
        m_TextArray[i].setString(dat[i].first);
        m_TextArray[i].setCharacterSize(characterSize);
        m_TextArray[i].setFillColor(sf::Color::Black); // ��� ����� ������ ����, ������� ��� �����

        m_TextArray[i].setPosition(sf::Vector2f(dat[i].second, 60) + newPosition); // ������������� ������� �� X �� ������� positionsX � �� Y �� ���������
    }
    // ��������� ������� � ������ �����
    bottomLineCommandsUI_1.setFillColor(sf::Color(128, 128, 128)); // ����� ����
    bottomLineCommandsUI_1.setSize(sf::Vector2f(background.getSize().x, 1)); // ������������� ������ ����� ������ ������ ����
    bottomLineCommandsUI_1.setPosition(newPosition.x, newPosition.y + 78); // ����� ��� ���������� �� 2 ������� ����
}

void NewDirectory::activeMenu() {
    // ���������� �������� � ���������� �����
    const sf::Color activeColor(100, 100, 250);
    const sf::Color inactiveColor(200, 200, 200);
    const sf::Color activeTextColor(sf::Color::White);
    const sf::Color inactiveTextColor(sf::Color::Black);

    // ������� ������ � ��������������� �������
    std::vector<sf::RectangleShape*> buttons = { &directoryButton, &DataCommandsButton, &optionsButton };
    std::vector<sf::Text*> texts = { &directoryText, &DataCommandsText, &optionsText };

    // ������������� ����� ��� ������ � �������
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
            //const float maxWidth = background.getSize().x - 38; // ������������ ������ ������
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
                // �������� ��������� "C:\\Users\\Aio\\Documents\\Warcraft III\\CustomMapData" �� ����
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
    directoryTriangles.clear(); // ������� ������������

    float yOffset = 60.f + scrollOffset; // ��������� scrollOffset ��� ������� yOffset
    std::function<void(const DirectoryEntry&, float)> addTexts = [&](const DirectoryEntry& dir, float indent) {
        if (dir.isOpen) {
            for (const auto& subDir : dir.subDirectories) {
                    sf::Text dirText(subDir.name, font, 14);
                    dirText.setFillColor(sf::Color::Black);
                    dirText.setPosition(sf::Vector2f(38 + indent * 20, yOffset) + newPosition); // �������� ������ ��� ���������� ������������
                    directoryTexts.push_back(dirText);

                    if (!subDir.subDirectories.empty()) { // ���������, ���� �� �����������
                        sf::ConvexShape triangle(3); // ������� �����������
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
                        directoryTriangles.push_back(triangle); // ��������� �����������
                    }

                    if (isBackgroundDirectory == false && subDir.fullPath == openDirectoryEntry.fullPath && (yOffset + newPosition.y >= 60 && yOffset <= background.getSize().y - 60 + newPosition.y)) {
                        backgroundDirectory.setPosition(sf::Vector2f(0, yOffset-6) + newPosition);
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

    // ������������� ������� thumb ����� ��������� ��� �������
    float scrollbarRange = scrollbar.getSize().y - scrollbarThumb.getSize().y;
    float scrollableArea = 30.0f * (directoryTexts.size() - visibleArea);
    float thumbPosition = (std::abs(scrollOffset) / scrollableArea) * scrollbarRange;
    thumbPosition = std::clamp(thumbPosition, 0.0f, scrollbarRange);
    scrollbarThumb.setPosition(sf::Vector2f(scrollbar.getPosition().x, scrollbar.getPosition().y + thumbPosition));
}

int NewDirectory::newDirectory() {
    std::filesystem::create_directory(pathDatMapPut);

    // �������� ����������� ���� ��� ������ ����� � ��������� ���� � ��������� �����
    std::wstring folderPath = run();

    // ��������, ���� �� ����
    if (folderPath == L"Exet") {
        if(m_DataAll.m_DataMaps.m_PutSaveCode.empty())
            m_DataAll.m_DataMaps.m_PutSaveCode = L"Exet";
        std::wcerr << "folderPath1.empty()" << std::endl;
        return 2;
    }

    // �������� ��������� "C:\\Users\\Aio\\Documents\\Warcraft III\\CustomMapData" �� ����
    size_t pos = folderPath.find(rootDirectory.fullPath);
    if (pos != std::wstring::npos) {
        folderPath.erase(pos, rootDirectory.fullPath.length());
    }

    // ��������, ���� �� ���������� ����
    if (folderPath.empty()) {
        std::wcerr << "folderPath2.empty()" << std::endl;
        return 0;
    }

    // �������� ������� ����
    std::wstring fullPath = pathDatMapPut + L'\\' + nameMaps;

    // �������� ���������� �� ����
    std::filesystem::create_directories(fullPath);

    // ���������� ����� ����� � ����
    fullPath += pathFile;

    std::ofstream outFile(fullPath, std::ios::binary);

    if (outFile) {
        StringConvector StringConvector_;
        std::string utf8Text = StringConvector_.utf16_to_utf8(folderPath);
        outFile.write(utf8Text.c_str(), utf8Text.size());
        
        outFile.close();
        std::wcout << "New path SaveCode ( " << pathDatMapPut + L'\\' + nameMaps << pathFile << L" )" << std::endl;
        if (utf8Text == "False")
            return 3;
        m_DataAll.m_DataMaps.m_PutSaveCode = folderPath;
        return 1; // �������� ����������
    }
    else {
        // ������ ��� �������� �����
        std::wcerr << "Failed to create file: " << pathDatMapPut << L'\\' << nameMaps << pathFile << L'\n';
        return 0;
    }
    return 0;
}

void NewDirectory::drawWindow() {
    window.clear(sf::Color(0, 255, 0));
    window.draw(background);

    if (numMenu == 1) {

        for (const auto& dirText : directoryTexts) { // ������� ������ ������
            if (dirText.getPosition().y >= topLine.getPosition().y - 30 && dirText.getPosition().y <= bottomLine.getPosition().y + 10) {
                window.draw(dirText);
            }
        }

        for (const auto& triangle : directoryTriangles) { // ����� ������ ������������
            // ��������� ���������� Y ���� ����� ������������
            float triangleY = (triangle.getPoint(0).y + triangle.getPoint(1).y + triangle.getPoint(2).y) / 3; // ������� Y ����������
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
        window.draw(bottomLineCommandsUI_1);

        for (CommandsUI_1& p : m_CommandsUI_1)
            p.drawDataCommands(window);
    }
    else if (numMenu == 3) {
        for (OptionsUI_1& p : m_OptionsUI_1)
            p.drawRow(window);
    }


    window.draw(topBackground);
    window.draw(bottomBackground);

    window.draw(topLine);
    window.draw(bottomLine);

    window.draw(title);

    window.draw(directoryButton);
    window.draw(directoryText);
    window.draw(DataCommandsButton);
    window.draw(DataCommandsText);
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

    // ���������� �������� ��� ������ ������ ����
    std::map<sf::RectangleShape*, std::function<void()>> menuActions{
        {&directoryButton, [&]() {
            if (numMenu != 1) {
                numMenu = 1;
                activeMenu();
                title.setString(L"Maps: " + nameMaps);
            }
        }},
        {&DataCommandsButton, [&]() {
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

    // ���������, ������ �� ���� �� ������ ����
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
    if (numMenu == 2) {
        short isSquare = 0;
        int numDataCommands = -1;
        for (int i = 0; i < m_CommandsUI_1.size(); ++i) {
            if (m_CommandsUI_1[i].square.getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
                numDataCommands = i;
                isSquare = 1;
                break;
            }
            else if (m_CommandsUI_1[i].square2.getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
                numDataCommands = i;
                isSquare = 2;
                break;
            }
            else if (m_CommandsUI_1[i].button.getGlobalBounds().contains(mouseButton.x, mouseButton.y)) {
                numDataCommands = i;
                isSquare = 3;
                break;
            }
        }

        if (isSquare != 0) {

            int hotKeyNew = '\0';
            auto& cmdData = m_CommandsUI_1[numDataCommands];
            if (isSquare == 1) {
                cmdData.value1 = !cmdData.value1;
            }
            else if (isSquare == 2) {
                cmdData.value2 = !cmdData.value2;
            }
            //else if (isSquare == 3) {
            //    bool isRunning = true;
            //    while (isRunning) {
            //        for (int key = 8; key <= 255; key++) {
            //            if (GetAsyncKeyState(key) & 0x8000) {
            //                if (key == VK_BACK) {
            //                    hotKeyNew = 0;
            //                    isRunning = false;
            //                    break;
            //                }
            //                else {
            //                    hotKeyNew = static_cast<int>(key);
            //                    isRunning = false;
            //                    break;
            //                }
            //            }
            //        }
            //        Sleep(50); // ��������� ���, ����� �� ��������� ���������
            //    }
            //}

            std::vector<std::function<void()>> DataCommands = {
                [&] { m_DataAll.m_DataCommands.return1Cmd.isVisibleButton = !m_DataAll.m_DataCommands.return1Cmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.return2Cmd.isVisibleButton = !m_DataAll.m_DataCommands.return2Cmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.return3Cmd.isVisibleButton = !m_DataAll.m_DataCommands.return3Cmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.saveCmd.isVisibleButton = !m_DataAll.m_DataCommands.saveCmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.craftCmd.isVisibleButton = !m_DataAll.m_DataCommands.craftCmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.camCmd.isVisibleButton = !m_DataAll.m_DataCommands.camCmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.statsCmd.isVisibleButton = !m_DataAll.m_DataCommands.statsCmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.clearCmd.isVisibleButton = !m_DataAll.m_DataCommands.clearCmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.cleanCmd.isVisibleButton = !m_DataAll.m_DataCommands.cleanCmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.strCmd.isVisibleButton = !m_DataAll.m_DataCommands.strCmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.agiCmd.isVisibleButton = !m_DataAll.m_DataCommands.agiCmd.isVisibleButton; },
                [&] { m_DataAll.m_DataCommands.intCmd.isVisibleButton = !m_DataAll.m_DataCommands.intCmd.isVisibleButton; }
            };

            std::vector<std::function<void()>> loadDataCommands = {
                [&] { m_DataAll.m_DataCommands.return1Cmd.isLoad = !m_DataAll.m_DataCommands.return1Cmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.return2Cmd.isLoad = !m_DataAll.m_DataCommands.return2Cmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.return3Cmd.isLoad = !m_DataAll.m_DataCommands.return3Cmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.saveCmd.isLoad = !m_DataAll.m_DataCommands.saveCmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.craftCmd.isLoad = !m_DataAll.m_DataCommands.craftCmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.camCmd.isLoad = !m_DataAll.m_DataCommands.camCmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.statsCmd.isLoad = !m_DataAll.m_DataCommands.statsCmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.clearCmd.isLoad = !m_DataAll.m_DataCommands.clearCmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.cleanCmd.isLoad = !m_DataAll.m_DataCommands.cleanCmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.strCmd.isLoad = !m_DataAll.m_DataCommands.strCmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.agiCmd.isLoad = !m_DataAll.m_DataCommands.agiCmd.isLoad; },
                [&] { m_DataAll.m_DataCommands.intCmd.isLoad = !m_DataAll.m_DataCommands.intCmd.isLoad; }
            };

            //std::vector<std::function<void()>> hotKeyVector = {
            //    [&] { m_DataAll.m_DataCommands.returnCmd.hotKey = hotKeyNew; },
            //    [&] { m_DataAll.m_DataCommands.saveCmd.hotKey = hotKeyNew; },
            //    [&] { m_DataAll.m_DataCommands.craftCmd.hotKey = hotKeyNew; },
            //    [&] { m_DataAll.m_DataCommands.camCmd.hotKey = hotKeyNew; },
            //    [&] { m_DataAll.m_DataCommands.statsCmd.hotKey = hotKeyNew; },
            //    [&] { m_DataAll.m_DataCommands.clearCmd.hotKey = hotKeyNew; },
            //    [&] { m_DataAll.m_DataCommands.cleanCmd.hotKey = hotKeyNew; },
            //    [&] { m_DataAll.m_DataCommands.strCmd.hotKey = hotKeyNew; },
            //    [&] { m_DataAll.m_DataCommands.agiCmd.hotKey = hotKeyNew; },
            //    [&] { m_DataAll.m_DataCommands.intCmd.hotKey = hotKeyNew; }
            //};

            if (isSquare == 1) {
                DataCommands[numDataCommands]();
            }
            else if (isSquare == 2) {
                loadDataCommands[numDataCommands]();
            }
            //else if (isSquare == 3) {
            //    hotKeyVector[numDataCommands]();
            //    cmdData.updateHotKey(hotKeyNew);
            //}

            cmdData.updateCheckText();
            LoadDataCommands LoadDataCommands_(m_DataAll);
            LoadDataCommands_.saveDataCommands();
        }
    }
    else if (numMenu == 3) {
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        auto checkAndToggle = [&](const int& index, bool& option, bool& isSave) {
            if (m_OptionsUI_1[index].square.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                m_OptionsUI_1[index].value = !m_OptionsUI_1[index].value;
                m_OptionsUI_1[index].updateCheckText();
                option = !option;
                isSave = true;
            }
            };

        bool isSave = false;
        checkAndToggle(0, m_DataAll.m_OptionsData.auto�lickerKey, isSave);
        checkAndToggle(1, m_DataAll.m_OptionsData.auto�lickerMaus, isSave);
        checkAndToggle(2, m_DataAll.m_OptionsData.autoSave, isSave);
        //checkAndToggle(3, m_DataAll.m_OptionsData.autoStart, isSave);
        checkAndToggle(4, m_DataAll.m_OptionsData.autoSkillsUpgrade, isSave);

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

        // ���������� ����������� �������� ��� ������� �������
        float thumbPosition = mousePosition.y - scrollbar.getPosition().y - mouseOffset;
        thumbPosition = std::clamp(thumbPosition, 0.0f, scrollbarRange);
        scrollbarThumb.setPosition(sf::Vector2f(scrollbar.getPosition().x, scrollbar.getPosition().y + thumbPosition) + newPosition);

        scrollOffset = -(thumbPosition / scrollbarRange) * scrollableArea;
        updateDirectoryTexts();
    }
}

bool NewDirectory::IsWindowInFocus(const HWND& hWnd) const {
    if (hWnd == NULL) {
        //std::wcout << L"��� ��������� ����!" << std::endl;
        return 0;
    }

    // ����� ��� ����� ����
    wchar_t windowTitle[256];

    // �������� ��� ����
    int length = GetWindowText(hWnd, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
    if (length == 0) {
        //std::wcout << L"�� ������� �������� ��� ����!" << std::endl;
        return 0;
    }

    // ���������� ��� ���� � ���������
    if (wcscmp(windowTitle, L"Warcraft III") != 0) {
        //std::wcout << L"��� ��������� ���� �� ��������� � 'Warcraft III'" << windowTitle << std::endl;
        return 0;
    }

    //std::wcout << windowTitle << std::endl;


    return true;
}

std::wstring NewDirectory::run() {
    bool isActive = true;
    while (window.isOpen()) {

        // �������� ���������� ��������� ����
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
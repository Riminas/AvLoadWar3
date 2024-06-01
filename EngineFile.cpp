#include <filesystem>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include <Windows.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "getMapOpen.h"
#include "EngineFile.h"
#include "load.h"
#include <TlHelp32.h>

void EngineFile::engineFile1(bool isLastSaveCode)
{
    if (!m_DataPatc.loadDataOptions())
        return;

	{
		getMapOpen getMapOpen_;
		m_FileName = getMapOpen_.getMapOpen1(m_DataPatc.getMaps());

        if(m_FileName == "Error")
            return;
	}

    if (!m_DataMaps.loadDataMaps(m_FileName)) {
        if(createFileDataMaps() == false)
            return;

        return;
    }
    bool isError = false;
    m_ListHero = m_DataPatc.getFileSaveCode1(m_DataMaps.getPutSaveCode(), isError);
    for (DataPatc::listHero& p : m_ListHero) {
        if (p.nameChar.empty())
            isLastSaveCode = true;
    }

    if (isError)
        return;
    

    if (m_ListHero.size() == 1 || isLastSaveCode == true) {
        engineTip1(isLastSaveCode);
        return;
    }

    engineTip2();
}

void EngineFile::engineTip1(const bool& isLastSaveCode) {
    std::string patc = "\0";

    if (m_ListHero.size() > 1 && isLastSaveCode == true) {
        std::filesystem::file_time_type latestTime = std::filesystem::file_time_type();

        for (auto& p : m_ListHero) {
            if (p.latestTime > latestTime) {
                latestTime = p.latestTime;
                patc = p.patc;
            }
        }
    }
    else {
        patc = m_ListHero[0].patc;
    }
    while (GetAsyncKeyState(18) & 0x8000 || GetAsyncKeyState(48) & 0x8000) {//ожидание отжатия клаишь ' Alt + 0 '
        Sleep(100);
    }

    load load_;
    load_.load1(patc);
}

void EngineFile::engineTip2() 
{
    sf::Font font; 
    font.loadFromFile("dataAvLoad\\arialmt.ttf");

    std::vector<sf::Text> listHeroText;
    int i = 1;
    listHeroText.push_back(sf::Text("Alt + 0 cancel", font, 16));
    for (auto& p : m_ListHero) {
        std::string str = "\tAlt + " + std::to_string(i) + " Prof( " + p.nameChar + " )";
        listHeroText.push_back(sf::Text(str, font, 16));
        i++;
    }
    sf::RenderWindow window2(sf::VideoMode(350, sf::VideoMode::getDesktopMode().height), "AVLoad", sf::Style::None);
    {
        window2.setPosition(sf::Vector2i(0, 0)); // Установка позиции окна в верхний левый угол
        SetWindowPos(window2.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        
        i = 0;
        for (sf::Text& txt : listHeroText) {
            txt.setPosition(30, 100 + 40 * i);
            window2.draw(txt);
            i++;
        }

        window2.display();

        window2.setFramerateLimit(1); // Ограничение FPS
    }

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            if (msg.wParam == 1) {
                window2.close();
                break;
            }
            else if (msg.wParam >= 2 && msg.wParam <= 10)
            {
                if (isProcessWarcraftIII() == 0) {
                    window2.close();
                    break;
                }

                while (GetAsyncKeyState(18) & 0x8000 || GetAsyncKeyState(47 + msg.wParam) & 0x8000) {//ожидание отжатия клаишь ' Alt + 0 '
                    Sleep(100);
                }
                if (m_ListHero.size() >= msg.wParam - 1) {
                    window2.close();
                    Sleep(100);
                    load load_;
                    load_.load1(m_ListHero[msg.wParam - 2].patc);
                    break;
                }
            }
        }
    }
}

bool EngineFile::isProcessWarcraftIII()// варкрафт запущен изи закрыт
{
    const std::wstring nameProces_136 = L"Warcraft III.exe";

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe)) {
        CloseHandle(hSnapshot);
        return 0;
    }

    do {
        if (pe.szExeFile == nameProces_136) {
            CloseHandle(hSnapshot);
            return 1;
        }
    } while (Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
    return 0;
}

bool EngineFile::createFileDataMaps() {
    //// Создаем окно
    //sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Application");
    //window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2 - window.getSize().x / 2, sf::VideoMode::getDesktopMode().height / 2 - window.getSize().y / 2));

    //// Создаем шрифт
    //sf::Font font;
    //font.loadFromFile("dataAvLoad\\arialmt.ttf");

    //sf::Text textTest, textPath;
    //textTest.setFont(font);
    //textTest.setString("Specify the path to the save file (you will need only 1 time)");
    //textTest.setCharacterSize(24);
    //textTest.setFillColor(sf::Color::White);
    //textTest.setPosition(40, 100);

    //textPath.setFont(font);
    //textPath.setString(m_DataPatc.getSave() + "\\name map\\file.txt");
    //textPath.setCharacterSize(24);
    //textPath.setFillColor(sf::Color::White);
    //textPath.setPosition(40, 150);

    //// Создаем кнопку для выбора пути
    //sf::RectangleShape buttonPath(sf::Vector2f(200, 50));
    //buttonPath.setFillColor(sf::Color(128, 128, 128));
    //buttonPath.setPosition(100, 400);

    //sf::Text textButtonPath;
    //textButtonPath.setFont(font);
    //textButtonPath.setString("Choose Path");
    //textButtonPath.setCharacterSize(18);
    //textButtonPath.setFillColor(sf::Color::White);
    //textButtonPath.setPosition(130, 415);

    //// Создаем кнопку для закрытия окна
    //sf::RectangleShape buttonCancel(sf::Vector2f(200, 50));
    //buttonCancel.setFillColor(sf::Color(128, 128, 128));
    //buttonCancel.setPosition(500, 500);

    //sf::Text textButtonCancel;
    //textButtonCancel.setFont(font);
    //textButtonCancel.setString("Cancel");
    //textButtonCancel.setCharacterSize(18);
    //textButtonCancel.setFillColor(sf::Color::White);
    //textButtonCancel.setPosition(550, 515);

    //// Основной цикл обработки событий
    //while (window.isOpen())
    //{
    //    sf::Event event;
    //    while (window.pollEvent(event))
    //    {
    //        if (event.type == sf::Event::Closed)
    //        {
    //            window.close();
    //        }
    //        else if (event.type == sf::Event::MouseButtonPressed)
    //        {
    //            if (event.mouseButton.button == sf::Mouse::Left)
    //            {
    //                // Обработка нажатия на кнопку выбора пути
    //                if (buttonPath.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
    //                {
    //                    // Открываем окно выбора пути
    //                    std::filesystem::path selectedPath = std::filesystem::current_path();
    //                    // Обработка выбранного пути
    //                }
    //                // Обработка нажатия на кнопку закрытия
    //                else if (buttonCancel.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
    //                {
    //                    window.close();
    //                }
    //            }
    //        }
    //    }

    //    // Очищаем окно
    //    window.clear();

    //    // Рисуем объекты
    //    window.draw(textTest);
    //    window.draw(textPath);
    //    window.draw(buttonPath);
    //    window.draw(textButtonPath);
    //    window.draw(buttonCancel);
    //    window.draw(textButtonCancel);

    //    // Отображаем окно
    //    window.display();
    //}

    //// Создаем текст
    //sf::Text text1("Specify the path to the save file (you will need only 1 time)", font, 24);
    //text1.setFillColor(sf::Color::White);
    //text1.setPosition(200, 50);

    //sf::Text text2((m_DataPatc.getSave() + "\\name map\\file.txt"), font, 24);
    //text2.setFillColor(sf::Color::White);
    //text2.setPosition(200, 100);

    //// Создаем кнопки
    //sf::RectangleShape confirmButton(sf::Vector2f(100, 50));
    //confirmButton.setFillColor(sf::Color(0, 255, 0, 128));
    //confirmButton.setPosition(200, 250);

    //sf::RectangleShape cancelButton(sf::Vector2f(100, 50));
    //cancelButton.setFillColor(sf::Color(255, 0, 0, 128));
    //cancelButton.setPosition(350, 250);

    //sf::Text cancelButtonText("Cancel", font, 18);
    //cancelButtonText.setFillColor(sf::Color::White);
    //cancelButtonText.setPosition(370, 260);

    //// Основной цикл
    //while (window.isOpen())
    //{
    //    sf::Event event;
    //    while (window.pollEvent(event))
    //    {
    //        if (event.type == sf::Event::Closed) {
    //            window.close();
    //            return false;
    //        }
    //        else if (event.type == sf::Event::MouseButtonPressed)
    //        {
    //            if (event.mouseButton.button == sf::Mouse::Left)
    //            {
    //                if (confirmButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
    //                {
    //                    std::ofstream file((m_DataMaps.getPutDatMapPut() + '\\' + m_DataMaps.getNameMaps()).c_str());
    //                    file << inputText;
    //                    file.close();
    //                    return true;
    //                }
    //                else if (cancelButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
    //                {
    //                    return false;
    //                    // Действие при нажатии на кнопку "Отмена"
    //                }
    //            }
    //        }
    //    }

    //    window.clear();
    //    window.draw(text1);
    //    window.draw(text2);
    //    window.draw(confirmButton);
    //    window.draw(cancelButton);
    //    window.draw(cancelButtonText);
    //    window.display();
    //}
//
//    //const int x = 600, y = 200;
//    //sf::RenderWindow window(sf::VideoMode(x, y), "new map", sf::Style::None);
//    //sf::Vector2f vf = sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
//    //window.setPosition(sf::Vector2i((vf.x / 2 - x/2), (vf.y/2 - y/2))); // Установка позиции окна в верхний левый угол
//    //SetWindowPos(window.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
//    //window.setFramerateLimit(30); // Ограничение FPS
//
//    //// Создаем текстовое поле
//    //sf::Font font;
//    //font.loadFromFile("dataAvLoad\\arialmt.ttf");
//
//    //sf::Text textInfo(m_FileName, font, 16);
//    //textInfo.setFillColor(sf::Color::White);
//    //textInfo.setPosition(x/5, 80);
//
//    //sf::Text textInput;
//    //textInput.setFont(font);
//    //textInput.setCharacterSize(12);
//    //textInput.setFillColor(sf::Color::White);
//    //textInput.setPosition(20, 120);
//
//    //bool isTyping = false;
//    //std::string inputText = "";
//
//    //// Основной игровой цикл
//    //while (window.isOpen())
//    //{
//    //    sf::Event event;
//    //    while (window.pollEvent(event))
//    //    {
//    //        if (event.type == sf::Event::Closed) {
//    //            window.close();
//    //            return false;
//    //        }
//    //        else if (event.type == sf::Event::TextEntered)
//    //        {
//    //            if (event.text.unicode < 128) // Обрабатываем только стандартные ASCII-символы
//    //            {
//    //                if (event.text.unicode == '\b') // Обработка нажатия на клавишу "Backspace"
//    //                {
//    //                    if (!inputText.empty())
//    //                        inputText.pop_back();
//    //                }
//    //                else
//    //                {
//    //                    inputText += static_cast<char>(event.text.unicode);
//    //                }
//    //                textInput.setString(inputText);
//    //            }
//    //        }
//    //    }
//
//    //    // Очищаем окно и рисуем текстовое поле
//    //    window.clear();
//    //    window.draw(textInfo);
//    //    window.draw(textInput);
//    //    window.display();
//    //}
    return true;
}



#include "Engine.h"
#include <iostream>
#include <locale>
#include <filesystem>
#include "ii.h"

int main()
{
    //setlocale(LC_ALL, "Russian");

    //std::vector<std::pair<std::pair<std::string, std::string>, double>> outInput = {
    //    {{"Narnia_RPG_3_10.4J_ENG_FIX", "Narnia_RPG_3_10.4J_ENG_FIX"}, 1.0 },
    //    {{"Narnia_RPG_3_10.4J_ENG_FIX", "Narnia_RPG_3_10.4J_ENG_FIX"}, 1.0 },
    //    {{"Narnia_RPG_3_9_ENG_FIX", "Narnia_RPG_3_10.4J_ENG_FIX"}, 1.0 },
    //    {{"Narnia_RPG_3_9_ENG_FIX", "Narnia_RPG_3_10.4J_ENG"}, 1.0 },
    //    {{"Narnia_RPG_2", "Narnia_RPG_3"}, 0.5 },
    //    {{"Eng_Mep_rpg_v5", "Mep_rpg"}, 1.0 },
    //    {{"Eng_Mep_rpg_v5", "Narnia_RPG_3_10.4J_ENG"}, 0.0 },
    //};

    ////std::string str1 = "35783_Narnia_RPG_3_10.4J_ENG_FIX.w3x";
    ////std::string str2 = "Narnia_RPG_3_10.4J_ENG_FIX.w3x";

    ////std::cout << "Строк start: " << str1 << std::endl;
    ////std::cout << "Строк start: " << str2 << std::endl;

    //std::cout << std::endl<< std::endl;

    //ii ii_;
    //while (true)
    //{
    //    std::cout << ii_.ii1(outInput[0].first.first, outInput[0].first.second, outInput[0].second) << std::endl;
    //    std::cin.get();
    //}
    Engine Engine_;
    Engine_.engine1();

    //return 0;
}

//#include <Windows.h>
////#undef min
//#include <SFML/Graphics.hpp>
//
//int main() {
//    // Создаем окно
//   sf::RenderWindow window(sf::VideoMode(800, 150), "SFML Application", sf::Style::None);
//   window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2 - window.getSize().x / 2, sf::VideoMode::getDesktopMode().height / 2 - window.getSize().y / 2));
//
//   // Создаем шрифт
//   sf::Font font;
//   font.loadFromFile("dataAvLoad\\arialmt.ttf");
//
//   sf::Text textTest, textPath;
//   textTest.setFont(font);
//   textTest.setString("Specify the path to the save file (you will need only 1 time)");
//   textTest.setCharacterSize(18);
//   textTest.setFillColor(sf::Color::White);
//   textTest.setPosition(40, 20);
//
//   textPath.setFont(font);
//   textPath.setString("path\\name map\\file.txt");
//   textPath.setCharacterSize(14);
//   textPath.setFillColor(sf::Color::White);
//   textPath.setPosition(80, 48);
//
//   // Создаем кнопку для выбора пути
//   sf::RectangleShape buttonPath(sf::Vector2f(25, 25));
//   buttonPath.setFillColor(sf::Color(128, 128, 128));
//   buttonPath.setPosition(40, 50);
//
//   // Создаем кнопку для закрытия окна
//   sf::RectangleShape buttonCancel(sf::Vector2f(100, 30));
//   buttonCancel.setFillColor(sf::Color(128, 128, 128));
//   buttonCancel.setPosition(window.getSize().x - 20 - 100, window.getSize().y - 20 - 30);
//
//   // Основной цикл обработки событий
//   while (window.isOpen())
//   {
//       sf::Event event;
//       while (window.pollEvent(event))
//       {
//           if (event.type == sf::Event::Closed)
//           {
//               window.close();
//           }
//           else if (event.type == sf::Event::MouseButtonPressed)
//           {
//               if (event.mouseButton.button == sf::Mouse::Left)
//               {
//                   // Обработка нажатия на кнопку выбора пути
//                   if (buttonPath.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
//                   {
//
//                   }
//                   // Обработка нажатия на кнопку закрытия
//                   else if (buttonCancel.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
//                   {
//                       window.close();
//                   }
//               }
//           }
//       }
//
//       // Очищаем окно
//       window.clear();
//
//       // Рисуем объекты
//       window.draw(textTest);
//       window.draw(textPath);
//       window.draw(buttonPath);
//       window.draw(buttonCancel);
//
//       // Отображаем окно
//       window.display();
//   }
//   return 0;
//}

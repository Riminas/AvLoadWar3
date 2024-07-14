#pragma once
#include"SFML/Graphics.hpp"
#include <string>
class StringConvector
{
public:
    std::string utf16_to_utf8(const std::wstring& wstr);
    std::wstring utf8_to_utf16(const std::string& str);
    void adjustTextToFit(sf::Text& text, float maxWidth);
    std::wstring toLower(const std::wstring& str);
};


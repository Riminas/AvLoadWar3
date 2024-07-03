#include <Windows.h>

#include "StringConvector.h"

std::string StringConvector::utf16_to_utf8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring  StringConvector::utf8_to_utf16(const std::string& str) {
    if (str.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

void StringConvector::adjustTextToFit(sf::Text& text, float maxWidth) {
    std::wstring str = text.getString().toWideString();
    if (str.empty()) return;

    const sf::Font font = *text.getFont();  // Получаем шрифт
    unsigned int characterSize = text.getCharacterSize();
    sf::Text tempText("", font, characterSize);

    tempText.setString(str);
    if (tempText.getLocalBounds().width <= maxWidth) {
        return;  // Текст уже помещается
    }

    std::wstring originalStr = str;
    while (tempText.getLocalBounds().width > maxWidth && !str.empty()) {
        str.pop_back();  // Удаляем последний символ
        tempText.setString(str + L"...");  // Добавляем многоточие для проверки ширины
    }

    if (tempText.getLocalBounds().width > maxWidth && !str.empty()) {
        str.pop_back();  // В случае, если текст все еще слишком длинный
    }

    text.setString(str + L"...");
}

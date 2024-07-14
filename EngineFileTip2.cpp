#include <wtypes.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <format>
#include <iomanip>
#include <tlhelp32.h>
#include "getMapOpen.h"
#include "LoadManager.h"
#include "EngineFileTip2.h"
#include "EngineFileTip1.h"
#include "NewDirectory.h"
#include "SkillsUpgradeStart.h"
#include "StringConvector.h"

EngineFileTip2::EngineFileTip2(sf::RenderWindow& t_Window, sf::Font& t_font, DataAll& t_DataAll)
    : m_Window(t_Window), font(t_font), m_DataAll(t_DataAll)
{
}

short EngineFileTip2::initialize() {
    if (!engineFile()) {
        return 0;
    }

    m_ListHeroDraw.resize(m_ListHero.size() + 1);
    unsigned int characterSize = 16; // Размер шрифта

    m_MaxNameWidth = 0;
    for (const auto& hero : m_ListHero) {
        if (hero.nameChar.length() > m_MaxNameWidth) {
            m_MaxNameWidth = hero.nameChar.length();
        }
    }

    for (int i = 0; i < m_ListHero.size(); ++i) {
        createHeroDraw(i, characterSize);
    }

    createHeroDraw(static_cast<int>(m_ListHero.size()), characterSize);

    isInitialize = true;
    return 1;
}

bool EngineFileTip2::engineFile() {
    bool isError = false;
    m_ListHero = m_DataAll.m_DataPath.retrieveHeroData(m_DataAll.m_DataMaps.getPutSaveCode(), isError);
    if (isError) return false;

    return true;
}

int EngineFileTip2::mouseButtonPressed(sf::Event& event, bool& isWindow2Visible) {
    const int lastNum = (int)m_ListHeroDraw.size() - 1;
    for (int i = 0; i < lastNum; ++i) {
        if (m_ListHeroDraw[i].shape.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
            return i;
        }
    }

    if (m_ListHeroDraw[lastNum].shape.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
        return -2;
    }
    return -1;
}

void EngineFileTip2::draw() {
    for (const ListHeroDraw& list : m_ListHeroDraw) {
        m_Window.draw(list.shape);
        m_Window.draw(list.text);
        m_Window.draw(list.textData);
    }
}

std::wstring EngineFileTip2::file_time_to_wstring(const std::filesystem::file_time_type& ftime) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()
    );

    std::time_t time = std::chrono::system_clock::to_time_t(sctp);
    std::tm tm;
    localtime_s(&tm, &time);

    std::wostringstream oss;
    oss << std::put_time(&tm, L"%d-%m-%Y\n    %H:%M");
    return oss.str();
}

void EngineFileTip2::activeGameTrue(const HWND& hWndWindow, const bool& isUpdataPosition) {
    RECT rect;
    if (GetClientRect(hWndWindow, &rect)) {
        const int width = rect.right - rect.left;
        const int height = rect.bottom - rect.top;
        const float x = width - 400.0f - 41.0f;
        float y = height / 4.0f * 3.0f - 20 - (m_ListHero.size() * 25.0f + 25.0f);

        if (y < 0) {
            y = (height % 25) / 2;

            int num = m_ListHero.size() * 25.0f;

            if (height < num) {
                int num2 = (height / 25.f);
                std::vector<DataPath::HeroInfo> listHero{};
                listHero.resize(num2);
                std::vector<ListHeroDraw> listHeroDraw{};
                listHeroDraw.resize(num2);
                int j = 0;
                for (size_t i = m_ListHero.size() - num2; auto & p : listHero) {
                    p = m_ListHero[i];
                    listHeroDraw[j] = m_ListHeroDraw[i+1];
                    i++;
                    j++;
                }
                m_ListHero = listHero;
                m_ListHeroDraw = listHeroDraw;
            }
        }

        sf::Vector2f newPosition = sf::Vector2f(x, y);

        if (isUpdataPosition)
            updatePosition(newPosition);
    }
}

void EngineFileTip2::updatePosition(const sf::Vector2f& newPosition) {
    for (unsigned i = 0; i < m_ListHeroDraw.size(); ++i) {
        m_ListHeroDraw[i].shape.setPosition(sf::Vector2f(0, static_cast<float>(2 + 25 * i)) + newPosition);
        m_ListHeroDraw[i].text.setPosition(sf::Vector2f(4, static_cast<float>(3 + 25 * i)) + newPosition);
        m_ListHeroDraw[i].textData.setPosition(sf::Vector2f(m_ListHeroDraw[i].shape.getSize().x-60, static_cast<float>(3 + 25 * i)) + newPosition);
        const float maxWidth = m_ListHeroDraw[i].shape.getSize().x - 60;
        StringConvector StringConvector_;
        StringConvector_.adjustTextToFit(m_ListHeroDraw[i].text, maxWidth);
    }
}

std::wstring EngineFileTip2::getPathListHero(const int& i) {
    return m_ListHero[i].path;
}

void EngineFileTip2::createHeroDraw(int index, unsigned int characterSize) {
    bool isLastElement = (index == m_ListHero.size());
    m_ListHeroDraw[index].shape.setSize(sf::Vector2f(400, 23));
    m_ListHeroDraw[index].shape.setFillColor(sf::Color::White);
    m_ListHeroDraw[index].shape.setOutlineColor(sf::Color::Black);
    m_ListHeroDraw[index].shape.setOutlineThickness(2);

    if (!isLastElement) {
        //std::wstring str = m_ListHeroDraw[index].text.getString();
        //if (str.length() > 40) {
        //    str.erase(40);
        //    str += L"...";
        //}
        m_ListHeroDraw[index].text.setString(m_ListHero[index].nameChar);
        m_ListHeroDraw[index].text.setFont(font);
        m_ListHeroDraw[index].text.setCharacterSize(characterSize);
        m_ListHeroDraw[index].text.setFillColor(sf::Color::Black);

        std::wstring strtData = file_time_to_wstring(m_ListHero[index].latestTime);
        m_ListHeroDraw[index].textData.setString(strtData);
        m_ListHeroDraw[index].textData.setFont(font);
        m_ListHeroDraw[index].textData.setCharacterSize(characterSize-6);
        m_ListHeroDraw[index].textData.setFillColor(sf::Color::Black);
        m_ListHeroDraw[index].textData.setLineSpacing(0.6f);
    }
    else {
        std::wstring str = std::format(L"{:>20}", L"Отменить");
        m_ListHeroDraw[index].text.setString(str);
        m_ListHeroDraw[index].text.setFont(font);
        m_ListHeroDraw[index].text.setCharacterSize(characterSize);
        m_ListHeroDraw[index].text.setFillColor(sf::Color::Black);

        m_ListHeroDraw[index].textData.setString(L"\0");
    }
}
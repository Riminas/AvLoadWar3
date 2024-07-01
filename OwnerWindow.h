#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "DataPath.h"
#include "OptionsData.h"
#include "LoadComands.h"

class OwnerWindow
{
public:
	OwnerWindow(sf::RenderWindow& t_Window, sf::Font& t_Font, DataPath& t_DataPath, bool& t_isExetTree, bool& t_IsVisibleLoad, OptionsData& t_OptionsData);
	void draw();
	void processingButton(const sf::Vector2i& event, bool isWindow2Visible[]);
	void processingButtonMenu(const sf::Vector2i& event, bool isWindow2Visible[]);
	void initialize();

	void activeGameTrue(HWND& hWndWindow);
	void activeGameFalse(HWND& hWndWindow);

	void setIsVisibleMenu(const bool& t_IsVisibleMenu);

private:
	void setupWindow();
	int mouseButtonPressed(const sf::Vector2i& event, bool isWindow2Visible[]);
	int mouseButtonMenuPressed(const sf::Vector2i& event, bool isWindow2Visible[]);
	void initializeButtonsComands();
	void updatePosition(const sf::Vector2f& newPoition);

	std::vector<Button> m_Buttons = std::vector<Button>(5);
	std::vector<Button> m_ButtonsMenu = std::vector<Button>(4);

	const sf::Vector2i m_ConstSize{20, 120};
	bool m_IsVisibleOwnerWindow = false;
	bool m_IsVisibleMenu = false;

	sf::RenderWindow& m_Window;
	sf::Font& m_Font;

	DataPath& m_DataPath;

	bool& isExetTree;

	sf::RectangleShape shapeIsLoad;
	sf::Text textureIsLoad;
	bool& m_IsVisibleLoad;
	OptionsData& m_OptionsData;
	Comands m_Comands;
};


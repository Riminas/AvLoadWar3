#pragma once
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "DataAll.h"

class OwnerWindow
{
public:
	OwnerWindow(sf::RenderWindow& t_Window, sf::Font& t_Font, DataAll& t_DataAll, bool& t_isExetTree, bool& t_IsVisibleLoad);
	void draw();
	void processingButton(const sf::Vector2i& event, bool isWindow2Visible[]);
	void processingButtonMenu(const sf::Vector2i& event, bool isWindow2Visible[]);
	void initialize();
	void initializeButtonsDataCommands();

	void activeGameTrue(const HWND& hWndWindow);
	void activeGameFalse();

	void setIsVisibleMenu(const bool& t_IsVisibleMenu);

private:
	void setupWindow();
	int mouseButtonPressed(const sf::Vector2i& event, bool isWindow2Visible[]);
	int mouseButtonMenuPressed(const sf::Vector2i& event, bool isWindow2Visible[]);
	void updateButtonsVisible();
	void updatePosition(const sf::Vector2f& newPoition, const sf::Vector2f& windowPoition, const sf::Vector2f& windowWidthHeight);

	std::vector<Button> m_Buttons/* = std::vector<Button>(5)*/;
	std::vector<Button> m_ButtonsMenu/* = std::vector<Button>(4)*/;
	sf::RectangleShape m_ShapeTrueVisibleMainMenu;
	sf::RectangleShape m_ShapeFalseVisibleMainMenu;
	Button m_ShapePlayerName;

	const sf::Vector2i m_ConstSize{20, 120};
	bool m_IsVisibleOwnerWindow = false;
	bool m_IsVisibleMenu = false;
	bool m_IsVisibleMainMenu = true;

	sf::RectangleShape shapeIsLoad;
	sf::Text textureIsLoad;

	sf::RenderWindow& m_Window;
	sf::Font& m_Font;
	DataAll& m_DataAll;
	bool& isExetTree;
	bool& m_IsVisibleLoad;
};


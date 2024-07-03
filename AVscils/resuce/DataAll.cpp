#include "getMapOpen.h"
#include "NewDirectory.h"
#include "LoadCommands.h"
#include "DataAll.h"


NewDataAll::NewDataAll(DataAll& t_DataAll, sf::RenderWindow& t_Window, sf::Font& t_Font)
    : m_DataAll(t_DataAll),
    m_Window(t_Window),
    m_Font(t_Font)
{
}

void NewDataAll::newMaps(const bool& isNoCommands, const bool& isOptions)
{
	getMapOpen getMapOpen_;
	const std::wstring newNameMaps = getMapOpen_.getMapOpen1(m_DataAll.m_DataPath.getMapsPath());
    if (newNameMaps == L"Error") { m_DataAll.isNewMaps = false; return; }

	if (isOptions || m_DataAll.m_DataMaps.getNameMaps() != newNameMaps) {
        m_DataAll.m_DataMaps.NameMaps(newNameMaps);
        int num = m_DataAll.m_DataMaps.PutSaveCode(m_DataAll.m_DataPath.getSavePath());
        if (isNoCommands || isOptions || num == 0) {
            sf::Vector2f newPosition(0, 0);
            bool faic = false;
            NewDirectory app(m_Window, m_Font, newPosition, faic, m_DataAll);
            app.newDirectory();
        }
        else if (isNoCommands == false && num == 2) {

        }

        LoadCommands LoadCommands_(m_DataAll);
        LoadCommands_.loadCommands();

        m_DataAll.isNewMaps = true;
	}

	
}

void NewDataAll::newWarcrft(HWND& hWndWindow)
{
    m_DataAll.m_DataPath.m_hWndWindow = hWndWindow;
    if (!m_DataAll.m_DataPath.initializePaths(m_DataAll.versionWarcraft3)) {
        m_DataAll.isNewWarcrft = false;
        return;
    }
    m_DataAll.isNewWarcrft = true;
    
    newMaps();
}

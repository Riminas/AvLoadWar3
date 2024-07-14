#include "getMapOpen.h"
#include "NewDirectory.h"
#include "LoadDataCommands.h"
#include "DataAll.h"


NewDataAll::NewDataAll(DataAll& t_DataAll, sf::RenderWindow& t_Window, sf::Font& t_Font)
    : m_DataAll(t_DataAll),
    m_Window(t_Window),
    m_Font(t_Font)
{
}

void NewDataAll::newMaps(const bool& isNoDataCommands, const bool& isOptions)
{
	getMapOpen getMapOpen_;
	const std::wstring newNameMaps = getMapOpen_.getMapOpen1(m_DataAll.m_DataPath.getMapsPath());
    if (newNameMaps == L"Error") { 
        m_DataAll.isNewMaps = false;
        m_DataAll.isMapsStart = true;
        //m_DataAll.isMapsStart = false;
        return; 
    }
    else {
        m_DataAll.isMapsStart = true;
    }

    bool isNewMaps = false;
    const bool isNewNameMaps = m_DataAll.m_DataMaps.m_NameMapsFull != newNameMaps;
	if (isNoDataCommands || isOptions || isNewNameMaps) {
        if (isNewNameMaps) {
            m_DataAll.m_DataMaps.m_NameMapsFull = newNameMaps;
            m_DataAll.m_DataMaps.NameMaps();
            m_DataAll.m_IsUpdataOwnerWindow = true;
        }

        const bool isNewDirectory{ (m_DataAll.isNewMaps == false && isNoDataCommands) || isOptions };
        if (isNewDirectory) {

            //int num = m_DataAll.m_DataMaps.PutSaveCode(m_DataAll.m_DataPath.getSavePath());
            if (isNewDirectory/* || (isNoDataCommands && num == 2)*/) {

                sf::Vector2f newPosition(0, 0);
                bool faic = false;
                NewDirectory app(m_Window, m_Font, newPosition, faic, m_DataAll);

                const int num2 = app.newDirectory();
                if (num2 == 3) { // if (utf8Text == "False") return 3; убрать выбор дириктории
                    m_DataAll.m_DataMaps.m_PutSaveCode = L"False";
                    isNewMaps = false;
                }
                else if (num2 == 2) { // if (folderPath == L"Exet") return 2;
                }
                else if (num2 == 1) { //true
                    //if (m_DataAll.m_DataMaps.m_PutSaveCode == L"False")
                    //    m_DataAll.m_DataMaps.m_PutSaveCode = L"\0";
                    isNewMaps = true;
                }
                else if (num2 == 0) {// false
                    m_DataAll.m_DataMaps.m_PutSaveCode = L"False";
                    isNewMaps = false;
                }
                if (m_DataAll.m_DataMaps.m_LastPathSaveCode != m_DataAll.m_DataMaps.m_PutSaveCode) {
                    m_DataAll.m_DataMaps.m_LastPathSaveCode = m_DataAll.m_DataMaps.m_PutSaveCode;
                    m_DataAll.m_DataMaps.m_IsNewInfo = true;
                }
                else {
                    isNewMaps = false;
                }
            }
            //else if (isNoDataCommands == false && num == 2) {
            //    m_DataAll.m_DataMaps.m_IsNewInfo = true;
            //}
        }

        LoadDataCommands LoadDataCommands_(m_DataAll);
        LoadDataCommands_.loadDataCommands();

        m_DataAll.isNewMaps = isNewMaps;
        if (!isOptions) {
            m_DataAll.m_OptionsData.isStaitAuto = false;
        }
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

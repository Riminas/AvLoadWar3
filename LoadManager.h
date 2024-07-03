#pragma once
#include <string>
#include <wtypes.h>
#include <regex>

class LoadManager {
public:
    LoadManager(HWND& hWnd);
    bool executeLoad(const std::wstring& filePath);
    void sendLoadCommands(const std::vector<std::string>& codes, const bool& isDelay = true, bool isLastLoad = false);
private:
    bool switchToTargetWindow();
    bool IsWindowInFocus(HWND hWnd);
    bool loadCodesFromFile(std::vector<std::string>& codes);
    void processSingleCode(std::string& code, const std::smatch& match, const std::string& prefixToRemove = "");
    void simulateKeyStrokes(const std::string& code);
    void setClipboardText(const std::string& text);
    void simulatePaste();
    void displayCommand(const std::string& command);
    bool checkKeyPressed();
    //bool switchInputLanguage(DWORD language);

    const HWND& m_TargetWindow;
    short m_LoadType = 0;
    std::wstring m_FilePath = L"\0";
    bool isFistLoad = true;
};


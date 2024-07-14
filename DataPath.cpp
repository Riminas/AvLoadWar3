#include <iostream>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <unordered_map>
#include <Windows.h>
#include <ShlObj_core.h>
#include <unordered_set>
#include <shlobj.h>
#include <locale>
#include <codecvt>
#include <psapi.h>

#include "StringToString.h"
#include "StringConvector.h"
#include "NewPathSaveCode.h"
#include "LoadDataFail.h"

#include "DataPath.h"

// Utility function to remove specific characters from a string
void removeSpecialCharacters(std::wstring& wstr) {
    std::wstring wstr2 = wstr;
    wstr = L"\0";
    for (wchar_t& wc : wstr2) {
        if (wc == L'(' || wc == L')' || wc == L':' || wc == L'"') {
        }
        else {
            wstr += wc;
        }
    }
}

std::wstring DataPath::openWarcraft3() {
    //// Получаем дескриптор активного окна
    ////HWND hWnd = GetTopWindow(NULL);
    ////HWND hWnd = GetFocus();
    //HWND hWnd = GetForegroundWindow();
    ////HWND hWnd = GetActiveWindow();
    //if (hWnd == NULL) {
    //    std::wcout << L"Нет активного окна!" << std::endl;
    //    return L"\0";
    //}

    //// Буфер для имени окна
    //wchar_t windowTitle[256];

    //// Получаем имя окна
    //int length = GetWindowText(hWnd, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
    //if (length == 0) {
    //    std::wcout << L"Не удалось получить имя окна!" << std::endl;
    //    return L"\0";
    //}

    //// Сравниваем имя окна с ожидаемым
    //if (wcscmp(windowTitle, L"Warcraft III") != 0) {
    //    std::wcout << L"Имя активного окна не совпадает с 'Warcraft III'" << windowTitle << std::endl;
    //    return L"\0";
    //}

    DWORD processId;
    GetWindowThreadProcessId(m_hWndWindow, &processId);
    
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess == NULL) {
        std::wstring path;
        NewPathSaveCode NewPathSaveCode_;
        if (NewPathSaveCode_.newPathSaveCode(path)) {
            return path;
        }

        std::wcout << L"Unable to open process!" << std::endl;
        return L"\0";
    }
    
    WCHAR processName[MAX_PATH];
    if (GetModuleFileNameEx(hProcess, NULL, processName, MAX_PATH)) {
        std::wcout << L"Process name: " << processName << std::endl;
        return processName;
    }
    else {
        std::wcout << L"Unable to get process name!" << std::endl;
    }

    CloseHandle(hProcess);
    return L"\0";
}

bool DataPath::initializePaths(int& versionWarcraft) {
    std::wstring pathWstr = L"\0";

    pathWstr = openWarcraft3();

    if(pathWstr.empty() || pathWstr == L"C:\\Windows\\explorer.exe")
        return false;
    

    std::wstring pathWar = L"\0";
    std::filesystem::path filePath(pathWstr);
    if (std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath)) {
        std::wstring filename = filePath.filename().wstring();

        if (filename == L"Warcraft III.exe") {
            std::wcout << L"Found: " << filePath << std::endl;
            versionWarcraft = 1;
        }
        else if (filename == L"war3.exe") {
            std::wcout << L"Found: " << filePath << std::endl;
            versionWarcraft = 2;
            pathWar = filePath.parent_path().wstring();
        }
        else {
            versionWarcraft = 0;
        }
    }

    if (versionWarcraft == 1) {
        PWSTR path = NULL;
        HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);
        if (!SUCCEEDED(hr)) {
            std::cout << "Error: SHGetKnownFolderPath()" << std::endl;
            return false;
        }

        // Преобразование wchar_t* в std::wstring
        StringConvector StringConvector_;
        std::string documentsPath = StringConvector_.utf16_to_utf8(path);

        // Освобождаем память, выделенную SHGetKnownFolderPath
        CoTaskMemFree(path);

        // Преобразование std::wstring в std::string с использованием UTF-8 кодировки

        std::wstring str = StringConvector_.utf8_to_utf16(documentsPath) + L"\\Warcraft III";

        if (!std::filesystem::is_directory(str)) {
            std::wcout << "Error: directory Warcraft III(" << str << ")" << std::endl;
            return false;
        }

        m_Maps = str + L"\\Maps";
        std::wcout << "Warcraft Maps Path: " << m_Maps << std::endl;
        m_Save = str + L"\\CustomMapData";
        std::wcout << "Warcraft Save Path: " << m_Save << std::endl;
    }
    else if (versionWarcraft == 2) {

        m_Maps = pathWar + L"\\Maps";
        std::wcout << "Warcraft Maps Path: " << m_Maps << std::endl;
        m_Save = pathWar;
        std::wcout << "Warcraft Save Path: " << m_Save << std::endl;
    }
    //std::cin.get();
    //std::cin.get();
    //std::cin.get();
    //std::cin.get();
    return true;
}

std::vector<DataPath::HeroInfo> DataPath::retrieveHeroData(const std::wstring& saveCodePath, bool& isError) {
    
    std::wstring fullSavePath = m_Save + saveCodePath;

    if (!std::filesystem::is_directory(fullSavePath)) {
        std::wcout << L"Error: directory - (" << fullSavePath << L")" << std::endl;
        isError = true;
        return m_HeroList;
    }

    m_HeroList = parseHeroData(fullSavePath);

    sortList(m_HeroList);

    return m_HeroList;
}


void  DataPath::sortList(std::vector<DataPath::HeroInfo>& heroList) {
    std::sort(heroList.begin(), heroList.end(), [](const DataPath::HeroInfo& a, const DataPath::HeroInfo& b) {
        return a.latestTime < b.latestTime; // Sort by latestTime in descending order
        });
}

std::vector<DataPath::HeroInfo> DataPath::parseHeroData(const std::wstring& folderPath) {
    std::unordered_map<std::wstring, HeroInfo> heroDataMap;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_directory()) {
            processDirectory(entry, heroDataMap);
        }
        else if (isTextFile(entry)) {
            processFile(entry, heroDataMap);
        }
    }

    return convertToHeroList(heroDataMap);
}

void DataPath::processDirectory(const std::filesystem::directory_entry& dirEntry, std::unordered_map<std::wstring, HeroInfo>& heroDataMap) {
    HeroInfo& heroInfo = heroDataMap[dirEntry.path().filename().wstring()];

    for (const auto& entry : std::filesystem::recursive_directory_iterator(dirEntry.path())) {
        if (isTextFile(entry)) {
            updateHeroInfo(entry, heroInfo);
        }
    }
}

void DataPath::processFile(const std::filesystem::directory_entry& fileEntry, std::unordered_map<std::wstring, HeroInfo>& heroDataMap) {


    //// Открытие файла
    //std::ifstream file(fileEntry.path(), std::ios::binary);
    //if (!file.is_open()) {
    //    return; // Если не удалось открыть файл, выйти из функции
    //}

    //// Чтение содержимого файла в буфер
    //std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    //std::string utf8Text(buffer.begin(), buffer.end());

    //// Удаление специальных символов из текста
    //removeSpecialCharacters(utf8Text);

    //// Преобразование текста из UTF-8 в UTF-16
    //
    //const std::wstring line = StringConvector_.utf8_to_utf16(utf8Text);

    //// Разделение строки на токены
    //std::wistringstream iss(line);
    //std::vector<std::wstring> tokens((std::istream_iterator<std::wstring, wchar_t>(iss)), std::istream_iterator<std::wstring, wchar_t>());

    //std::vector<std::wstring> tokens;
    //std::wstring currentToken;
    //for (size_t i = 0; i < line.length(); ++i) {
    //    if (line[i] == L'\n') {
    //        if (!currentToken.empty()) {
    //            tokens.push_back(currentToken);
    //            currentToken.clear();
    //        }
    //        tokens.push_back(L"\n");
    //    }
    //    else {
    //        currentToken += line[i];
    //    }
    //}

    LoadDataFail LoadDataFail_;
    std::vector<std::wstring> tokens = LoadDataFail_.loadDataFailTokens(fileEntry.path());

    if (tokens.size() <= 1)
        return;

    StringConvector StringConvector_;
    // Получение времени последнего изменения файла
    const auto fileTime = fileEntry.last_write_time();

    // Поиск имени героя в токенах
    for (size_t i = 0; i < tokens.size(); ++i) {
        removeSpecialCharacters(tokens[i]);
        if (isHeroToken(StringConvector_.toLower(tokens[i])) && isHeroTokenIsTrue(StringConvector_.toLower(tokens[i + 1]))) {
            std::wstring heroName = extractHeroName(tokens, i + 1);
            removeSpecialCharacters(heroName);

            // Обработка символа '|' в имени героя
            for (size_t j = 0; j < heroName.size(); ++j) {
                if (heroName[j] == L'|') {
                    if (heroName[j + 1] == L'r' || heroName[j + 1] == L'R') {
                        heroName.erase(j, 2);
                        --j;
                    }
                    else if (heroName[j + 1] == L'c' || heroName[j + 1] == L'C') {
                        heroName.erase(j, 10);
                    }
                }
            }

            // Обновление информации о герое
            HeroInfo& heroInfo = heroDataMap[heroName];
            if (fileTime > heroInfo.latestTime) {
                heroInfo.latestTime = fileTime;
                heroInfo.path = fileEntry.path().wstring();
            }
            return;
        }
    }

    // Если имя героя не найдено, используем имя файла как имя героя
    HeroInfo& heroInfo = heroDataMap[fileEntry.path().filename().wstring()];
    if (fileTime > heroInfo.latestTime) {
        heroInfo.latestTime = fileTime;
        heroInfo.path = fileEntry.path().wstring();
    }
}

bool DataPath::isTextFile(const std::filesystem::directory_entry& entry) const {
    return std::filesystem::is_regular_file(entry) && entry.path().extension() == ".txt";
}

bool DataPath::isHeroToken(const std::wstring& token) const {
    static const std::unordered_set<std::wstring> heroTokens = { L"hero", L"job", L"char", L"character", L"перс" };
    return heroTokens.count(token) > 0;
}

bool DataPath::isHeroTokenIsTrue(const std::wstring& token) const {
    static const std::unordered_set<std::wstring> heroTokens = { L"information", L"inventory" };
    return heroTokens.count(token) == 0;
}

std::wstring DataPath::extractHeroName(const std::vector<std::wstring>& tokens, size_t startIndex) const {
    std::wstring heroName = tokens[startIndex];
    for (size_t i = startIndex + 1; i < tokens.size(); ++i) {
        if (tokens[i] == L"call" || tokens[i] == L"Level")
            break;
        heroName += L" " + tokens[i];
    }
    return heroName;
}

void DataPath::updateHeroInfo(const std::filesystem::directory_entry& fileEntry, HeroInfo& heroInfo) {
    std::filesystem::file_time_type fileTime = fileEntry.last_write_time();
    if (fileTime > heroInfo.latestTime) {
        heroInfo.latestTime = fileTime;
        heroInfo.path = fileEntry.path().wstring();
    }
}

std::vector<DataPath::HeroInfo> DataPath::convertToHeroList(const std::unordered_map<std::wstring, HeroInfo>& heroDataMap) const {
    std::vector<HeroInfo> heroList;
    for (const auto& [name, info] : heroDataMap) {
        heroList.push_back(HeroInfo(name, info.path, info.latestTime));
    }
    return heroList;
}

std::wstring DataPath::getMapsPath() const { return m_Maps; }
std::wstring DataPath::getSavePath() const { return m_Save; }
std::vector<DataPath::HeroInfo> DataPath::getHeroList() const { return m_HeroList; }

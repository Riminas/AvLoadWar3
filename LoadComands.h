#pragma once
#include <string>
#include "DataPath.h"
#include "DataMaps.h"

class Comands {
public:
    bool load = false;
    struct Comnand {
        std::string str;
        bool isVisibleButton;
        bool isLoad;

        Comnand(std::string t_str, bool t_isVisibleButton, bool t_isLoad)
            : str(t_str), isVisibleButton(t_isVisibleButton), isLoad(t_isLoad) {}
    };

    Comnand returnCmd{ "-suicide", true, false };
    Comnand saveCmd{ "-save", true, false };
    Comnand craftCmd{ "-create", true, false };
    Comnand camCmd{ "-cam 150", true, false };

    Comnand statsCmd{ "-stats", true, false };
    Comnand clearCmd{ "-clear", true, false };
    Comnand cleanCmd{ "-clean", true, false };
    Comnand strCmd{ "-str 10", true, false };
    Comnand agiCmd{ "-agi 10", true, false };
    Comnand intCmd{ "-int 10", true, false };

    std::unordered_map<std::string, Comnand*> commandMap = {
        {"[Return]", &returnCmd},
        {"[Save]", &saveCmd},
        {"[Craft]", &craftCmd},
        {"[Cam]", &camCmd},
        {"[Stats]", &statsCmd},
        {"[Clear]", &clearCmd},
        {"[Clean]", &cleanCmd},
        {"[Str]", &strCmd},
        {"[Agi]", &agiCmd},
        {"[Int]", &intCmd}
    };
};

class LoadComands {
public:
    LoadComands(DataPath& t_DataPath) : m_DataPath(t_DataPath) {}
    Comands loadComands();
    void saveComands(Comands& comands);
private:
    std::wstring m_FileName = L"\0";
    std::wstring m_FullPath = L"\0";

    const std::wstring m_putDatMapPut = L"DataMaps";
    const std::wstring m_ComandsPath = L"\\Comands.dat";

    DataPath& m_DataPath;
    DataMaps m_DataMaps;

    void writeDefaultCommands(const std::wstring& fullPath, Comands& comands);
    void parseCommandLine(const std::string& line, Comands& comands);
    bool starComands();
};
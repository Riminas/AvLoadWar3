#pragma once
#include <unordered_map>
#include <string>
#include <wtypes.h>

class DataCommands {
public:
    bool load = false;

    struct Command {
        std::string str;
        bool isVisibleButton;
        bool isLoad;
        int hotKey;

        Command(std::string t_str, bool t_isVisibleButton, bool t_isLoad, int t_hotKey)
            : str(t_str), isVisibleButton(t_isVisibleButton), isLoad(t_isLoad), hotKey(t_hotKey) {}
    };

    Command return1Cmd{ "-suicide", true, false, 0 };
    Command return2Cmd{ "-2", false, false, 0 };
    Command return3Cmd{ "-3", false, false, 0 };
    Command saveCmd{ "-save", true, false, 0x34 };
    Command craftCmd{ "-create", true, false, 0 };
    Command camCmd{ "-cam 150", true, false, 0 };
    Command statsCmd{ "-stats", false, false, 0x35 };
    Command clearCmd{ "-clear", false, false, 0 };
    Command cleanCmd{ "-clean", false, false, 0 };
    Command strCmd{ "-str 10", false, false, 0 };
    Command agiCmd{ "-agi 10", false, false, 0 };
    Command intCmd{ "-int 10", false, false, 0 };

    std::unordered_map<std::string, Command> commandMapDefault = {
        {"[Return1]", return1Cmd},
        {"[Return2]", return2Cmd},
        {"[Return3]", return3Cmd},
        {"[Save]", saveCmd},
        {"[Craft]", craftCmd},
        {"[Cam]", camCmd},
        {"[Stats]", statsCmd},
        {"[Clear]", clearCmd},
        {"[Clean]", cleanCmd},
        {"[Str]", strCmd},
        {"[Agi]", agiCmd},
        {"[Int]", intCmd}
    };

    std::unordered_map<std::string, Command*> commandMap = {
        {"[Return1]", &return1Cmd},
        {"[Return2]", &return2Cmd},
        {"[Return3]", &return3Cmd},
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
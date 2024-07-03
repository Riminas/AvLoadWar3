#pragma once
#include <unordered_map>
#include <string>

class Commands {
public:
    bool load = false;

    struct Command {
        std::string str;
        bool isVisibleButton;
        bool isLoad;

        Command(std::string t_str, bool t_isVisibleButton, bool t_isLoad)
            : str(t_str), isVisibleButton(t_isVisibleButton), isLoad(t_isLoad) {}
    };

    Command returnCmd{ "-suicide", true, false };
    Command saveCmd{ "-save", true, false };
    Command craftCmd{ "-create", true, false };
    Command camCmd{ "-cam 150", true, false };
    Command statsCmd{ "-stats", false, false };
    Command clearCmd{ "-clear", false, false };
    Command cleanCmd{ "-clean", false, false };
    Command strCmd{ "-str 10", false, false };
    Command agiCmd{ "-agi 10", false, false };
    Command intCmd{ "-int 10", false, false };

    std::unordered_map<std::string, Command*> commandMap = {
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
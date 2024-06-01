#define WINVER 0x0500
#include <windows.h>
#include <vector>
#include <iostream>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <conio.h>
#include "key.h"

void key::key1(int c) {
    INPUT inputs{};

    // Pause for 5 seconds.

    // Set up a generic keyboard event.
    inputs.type = INPUT_KEYBOARD;
    inputs.ki.wScan = 0; // hardware scan code for key
    inputs.ki.time = 0;
    inputs.ki.dwExtraInfo = 0;



    inputs.ki.wVk = c; // virtual-key code for  key
    inputs.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &inputs, sizeof(INPUT));

    // Release  key
    inputs.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &inputs, sizeof(INPUT));
}

void key::key2(int c, int zajat) {
    INPUT inputs{};

    // Pause for 5 seconds.

    // Set up a generic keyboard event.
    inputs.type = INPUT_KEYBOARD;
    inputs.ki.wScan = 0; // hardware scan code for key
    inputs.ki.time = 0;
    inputs.ki.dwExtraInfo = 0;

    inputs.ki.wVk = zajat; // virtual-key code for the "Shift" or "ctrl" key
    inputs.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &inputs, sizeof(INPUT));

    inputs.ki.wVk = c; // virtual-key code for the "a" key
    inputs.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &inputs, sizeof(INPUT));

    // Release the "A" key
    inputs.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &inputs, sizeof(INPUT));

    inputs.ki.wVk = zajat; // virtual-key code for the "Shift" or "ctrl" key
    inputs.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &inputs, sizeof(INPUT));
}
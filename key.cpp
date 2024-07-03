#define WINVER 0x0500
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <conio.h>
#include "key.h"

void key::key1(int keyCode) {
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = keyCode;
    SendInput(1, &input, sizeof(INPUT));

    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void key::key2(int keyCode, int modifier) {
    INPUT inputs[2] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = modifier;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = keyCode;

    SendInput(2, inputs, sizeof(INPUT));

    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, inputs, sizeof(INPUT));
}

void key::keyVK(int keyCode, bool press) {
    INPUT input = {};
    input.type = INPUT_MOUSE;

    if (keyCode == VK_RBUTTON) {
        if (press) {
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        }
        else {
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        }
        SendInput(1, &input, sizeof(INPUT));
    }
    // �������� ������ ������ ����, ���� ����������, �������� VK_LBUTTON ��� ������ �����
}


void key::simulatePaste() {

    // ���������� ������� Ctrl+V ��� ������� ������
    INPUT inputs[4] = {};

    // ������� Ctrl
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    // ������� V
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'V';

    // ���������� V
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'V';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    // ���������� Ctrl
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    // �������� �������
    if (SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT)) != ARRAYSIZE(inputs)) {
        throw std::runtime_error("SendInput failed");
    }
}
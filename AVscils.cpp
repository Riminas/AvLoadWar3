#include "Engine.h"
#include <iostream>
#include <locale>
#include <filesystem>

#include "StringToString.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    //std::vector<std::pair<std::pair<std::string, std::string>, bool>> outInput = {
    //    {{"Narnia_RPG_3_10.4J_ENG_FIX", "Narnia_RPG_3_10.4J_ENG_FIX.w3x"}, true },
    //    {{"Narnia_RPG_3_10.4J_ENG_FIX", "43322_Narnia_RPG_3_10.4J_ENG_FIX"}, true },
    //    {{"Narnia_RPG_3_9_ENG_FIX", "Narnia_RPG_3_10.4J_ENG_FIX"}, true },
    //    {{"Narnia_RPG_3_9_ENG_FIX", "Narnia_RPG_3_10.4J_ENG"}, true },
    //    {{"11483_NARNIARPG2[Worl_nal_Eng_2.23.w3x", "Narnia_RPG_3"}, false },
    //    {{"11483_NARNIARPG2[Worl_nal_Eng_2.23.w3x", "NARNIARPG2[World]Final Eng 2.23.w3x"}, true },
    //    {{"Eng_Mep_rpg_v5", "Mep_rpg"}, true },
    //    {{"Eng_Mep_rpg_v5", "Narnia_RPG_3_10.4J_ENG"}, false },
    //    {{"43828_Fish_RPG_0.478.w3x", "432_Fish_RPG_0.478.w3x"}, true },
    //};

    //StringToString StringToString_;
    //StringToString_.testStringComparison(outInput);

    //std::cin.get();
    ////std::string str1 = "35783_Narnia_RPG_3_10.4J_ENG_FIX.w3x";
    ////std::string str2 = "Narnia_RPG_3_10.4J_ENG_FIX.w3x";

    ////std::cout << "Строк start: " << str1 << std::endl;
    ////std::cout << "Строк start: " << str2 << std::endl;

    //std::cout << std::endl<< std::endl;

    //ii ii_;
    //while (true)
    //{
    //    std::cout << ii_.ii1(outInput[0].first.first, outInput[0].first.second, outInput[0].second) << std::endl;
    //    std::cin.get();
    //}
    
    Engine Engine_;
    Engine_.engine1();

    return 0;
}
//
//#include <filesystem>
//#include <windows.h>
//#include <commdlg.h>
//#include <string>
//#include <iostream>
//
//// Функция для открытия диалогового окна выбора файла
//std::wstring OpenFileDialog() {
//    // Структура для хранения параметров диалогового окна
//    OPENFILENAME ofn;
//    wchar_t szFile[260] = { 0 };
//    ZeroMemory(&ofn, sizeof(ofn));
//    ofn.lStructSize = sizeof(ofn);
//    ofn.hwndOwner = NULL;  // Если окно имеет владельца, укажите его здесь
//    ofn.lpstrFile = szFile;
//    ofn.nMaxFile = sizeof(szFile);
//    ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
//    ofn.nFilterIndex = 1;
//    ofn.lpstrFileTitle = NULL;
//    ofn.nMaxFileTitle = 0;
//    ofn.lpstrInitialDir = NULL;
//    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//
//    // Открываем диалоговое окно
//    if (GetOpenFileName(&ofn) == TRUE) {
//        return std::wstring(szFile);
//    }
//    return L"";
//}
//
//std::string removePath(const std::string& filePath) {
//    std::string path = filePath;
//
//    // Найти начальный индекс для удаления
//    size_t start_pos = path.find("CustomMapData");
//    if (start_pos == std::string::npos) {
//        std::cerr << "Substring 'Warcraft III' not found in the path." << std::endl;
//        return "";
//    }
//
//    // Передвинем начальный индекс на конец строки "CustomMapData" (с учётом длины и обратного слэша)
//    start_pos += std::string("CustomMapData").length() + 1;
//
//    // Найти конечный индекс для удаления
//    size_t end_pos = path.find_last_of("\\");
//    if (end_pos == std::string::npos) {
//        std::cerr << "Last backslash not found in the path." << std::endl;
//        return "";
//    }
//
//    // Получить подстроку
//    return "\\" + path.substr(start_pos, end_pos - start_pos);
//    // Вывести результат
//}
//
//int main() {
//    std::wstring str = OpenFileDialog();
//    std::string filePath = { str.begin(), str.end() };
//    if (!filePath.empty()) {
//        std::cout << "Selected file: " << filePath << std::endl;
//    }
//    else {
//        std::cout << "No file selected." << std::endl;
//    }
//
//
//    filePath = removePath(filePath);
//    if (filePath.empty())
//        return 1;
//
//    std::cout << filePath << std::endl;
//
//
//    std::cin.get();
//    return 0;
//}

#include "Engine.h"
#include <iostream>
#include <locale>
#include <filesystem>
#include <fstream>

#include "StringToString.h"
#include <iostream>
#include <regex>
#include <string>



//bool containsOnlyEnglishCharacters(const std::string& text) {
//    // Регулярное выражение для проверки английских символов, цифр, пробелов, точек и дефисов
//    std::regex englishRegex("^[A-Za-z0-9 .\\$\\;\\@\\!\\^\\+\\=\\/\\<\\>\\#\\?\\*\\,\\\"\\\'\\&\\:\\_\\~\\(\\)\\-\\[\\]]*$");
//    return std::regex_match(text, englishRegex);
//}
//I am creating a program to load characters from Warcraft III : Reforged, specifically m16, which is the output of all heroes.I would appreciate your help gathering information about the map name and the path to save this map.
//(name(Narnia RPG 3 10.4J ENG FIX.w3x)   path(CustomMapData\NARNIARPG3))
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

    //int i = 0, skip = 66;
    //std::ifstream fil("mapsList.txt");
    //while (fil)
    //{
    //    std::string str = "\0", str2;
    //    getline(fil, str);
    //    if (i >= skip) {
    //        if (!str.empty()) {
    //            StringToString StringToString_;
    //            str2 = StringToString_.strVectorToStr(str);
    //            std::cout << i << ") str ( " << str << "  |  " << str2 << " ) " << std::endl;
    //        }

    //        std::cin.get();
    //    }
    //    i++;
    //}

    //std::cout << i << ") end" << std::endl;
    //std::cin.get();

    //std::ofstream outFile("mapsList.txt");
    //if (!outFile) {
    //    std::cerr << "Ошибка при открытии файла для записи." << std::endl;
    //    return 1;
    //}


    //for (const auto& entry : std::filesystem::recursive_directory_iterator("Maps")) {
    //    if (entry.is_regular_file()) {
    //        std::string str;
    //        std::ifstream fil(entry.path());
    //        getline(fil, str);

    //        if (str.substr(0, 4) == "HM3W") str = str.substr(5);
    //        int i = 0;
    //        while (str[i] == '\0') { i++; }
    //        str = str.substr(i);

    //        //if (str[0] == '|') str = str.substr(10);


    //        i = 0;
    //        while (str[i] != '\0') { i++; }
    //        str = str.substr(0, i);

    //        for (int j = 0; j < str.size(); j++) {
    //            if (str[j] == '|') {
    //                if (str[j + 1] == 'r' || str[j + 1] == 'R') {
    //                    str = str.substr(0, j) + str.substr(j+2);
    //                    break;
    //                }
    //                else if (str[j+1] == 'c' || str[j+1] == 'C') {
    //                    str = str.substr(0, j) + str.substr(j + 10);
    //                }
    //            }
    //        }

    //        if (containsOnlyEnglishCharacters(str)) {
    //            outFile << str << std::endl;
    //            std::cout  << str << std::endl;
    //            std::cout << "_________________________________________________________________________" << std::endl;
    //        }
    //        else {
    //            outFile << entry.path().filename().string() << std::endl;
    //            std::cout << entry.path().filename().string() << std::endl;
    //            std::cout << "_________________________________________________________________________" << std::endl;
    //        }

    //        //std::cin.get();
    //    }
    //}
    //std::cout << "end" << std::endl;
    //std::cin.get();

    Engine Engine_;
    Engine_.engine1();

    return 0;
}
//
////#include <filesystem>
////#include <windows.h>
////#include <commdlg.h>
////#include <string>
////#include <iostream>
////
////// Функция для открытия диалогового окна выбора файла
////std::wstring OpenFileDialog() {
////    // Структура для хранения параметров диалогового окна
////    OPENFILENAME ofn;
////    wchar_t szFile[260] = { 0 };
////    ZeroMemory(&ofn, sizeof(ofn));
////    ofn.lStructSize = sizeof(ofn);
////    ofn.hwndOwner = NULL;  // Если окно имеет владельца, укажите его здесь
////    ofn.lpstrFile = szFile;
////    ofn.nMaxFile = sizeof(szFile);
////    ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
////    ofn.nFilterIndex = 1;
////    ofn.lpstrFileTitle = NULL;
////    ofn.nMaxFileTitle = 0;
////    ofn.lpstrInitialDir = NULL;
////    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
////
////    // Открываем диалоговое окно
////    if (GetOpenFileName(&ofn) == TRUE) {
////        return std::wstring(szFile);
////    }
////    return L"";
////}
////
////std::string removePath(const std::string& filePath) {
////    std::string path = filePath;
////
////    // Найти начальный индекс для удаления
////    size_t start_pos = path.find("CustomMapData");
////    if (start_pos == std::string::npos) {
////        std::cerr << "Substring 'Warcraft III' not found in the path." << std::endl;
////        return "";
////    }
////
////    // Передвинем начальный индекс на конец строки "CustomMapData" (с учётом длины и обратного слэша)
////    start_pos += std::string("CustomMapData").length() + 1;
////
////    // Найти конечный индекс для удаления
////    size_t end_pos = path.find_last_of("\\");
////    if (end_pos == std::string::npos) {
////        std::cerr << "Last backslash not found in the path." << std::endl;
////        return "";
////    }
////
////    // Получить подстроку
////    return "\\" + path.substr(start_pos, end_pos - start_pos);
////    // Вывести результат
////}
////
////int main() {
////    std::wstring str = OpenFileDialog();
////    std::string filePath = { str.begin(), str.end() };
////    if (!filePath.empty()) {
////        std::cout << "Selected file: " << filePath << std::endl;
////    }
////    else {
////        std::cout << "No file selected." << std::endl;
////    }
////
////
////    filePath = removePath(filePath);
////    if (filePath.empty())
////        return 1;
////
////    std::cout << filePath << std::endl;
////
////
////    std::cin.get();
////    return 0;
////}

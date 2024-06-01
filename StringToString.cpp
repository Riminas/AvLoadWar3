#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <Windows.h>
#undef min
#include <cctype>
#include <random>
#include "StringToString.h"

std::string StringToString::removeOnString(const std::string t_str) {
    std::string str;
    for (const char& c : t_str) {
        str += std::tolower(c);
    }

    if (str == "zm666") return "zombie666";
    std::string str2;
    {
        str2 = str.substr(0, 3);
        if (str2 == "eng") return str2;
    }
    {
        str2 = str.substr(0, 5);
        if (str2 == "30min") return str2;
    }
    {
        str2 = str.substr(0, 6);
        if (str2 == "disrpg") return str2;
    }
    {
        str2 = str.substr(0, 9);
        if (str2 == "AnimalRPG") return str2;
    }

    return str;
}

bool StringToString::isIsclu(const std::string& word) {
    std::string str = "\0";
    for (const char& c : word) {
        str += std::tolower(c);
    }

    {
        if (str.size() > 2) {
            if (str[0] == 'v' && (str[1] >= 48 && str[1] <= 57)) return true;
        }
    }

    //list 
    {
        if (str == "eng") return true;
        else if (str == "bug") return true;
        else if (str == "hotfix") return true;
        else if (str == "jn") return true;
        else if (str == "world") return true;
        else if (str == "worl") return true;
        else if (str == "raid") return true;
        else if (str == "rai") return true;
        else if (str == "final") return true;
        else if (str == "nal") return true;
        else if (str == "test") return true;
        else if (str == "rus") return true;
        else if (str == "ru") return true;
        else if (str == "test") return true;
        else if (str == "translated") return true;
        else if (str == "ranslated") return true;
    }

    {
        std::string str2 = str.substr(0, 3);
        if (str2 == "fix") return true;
    }

    {
        std::string str2 = str.substr(0, 13);
        if (str2 == "translatedfix") return true;
        
    }

    return false;
}

std::vector<std::string> StringToString::removeString(const std::string& t_fileName)
{
    std::string fileName = t_fileName;
    std::unordered_map<std::string, int> mapString;

    for (char& c : fileName) {
        if (c == '_' || c == '-' || c == '(' || c == ')' || c == '[' || c == ']')
            c = ' ';
    }


    const size_t strSize = fileName.size();
    const std::string pref = fileName.substr(strSize - 4);
    if (pref == ".w3x") {
        fileName.replace(strSize - 4, strSize, "");
    }

    {
        std::stringstream ss(fileName);
        std::string word;
        while (ss >> word) {
            bool isAllNumber = true;
            bool isOneNumber = false;
            for (const char& c : word) {
                if (c >= 65 && c <= 90 || c >= 97 && c <= 122) {
                    if (isOneNumber) {
                        isAllNumber = false;
                        break;
                    }
                    isOneNumber = true;
                }
            }
            if (!isIsclu(word)) {
                if (!isAllNumber) 
                {
                    std::string str2 = removeOnString(word);
                    if (!isIsclu(str2)) {
                        std::string strWordToChar = str2;
                        mapString[strWordToChar]++;
                    }
                }
            }
        }
    }

    std::vector<std::string> strReturn;
    for (auto& p : mapString) {
        strReturn.push_back(p.first.c_str());
    }

    return strReturn;
}


//int levenshteinDistance(const std::string& s1, const std::string& s2) {
//    const size_t len1 = s1.size(), len2 = s2.size();
//    std::vector<std::vector<int>> d(len1 + 1, std::vector<int>(len2 + 1));
//
//    d[0][0] = 0;
//    for (size_t i = 1; i <= len1; ++i) d[i][0] = i;
//    for (size_t i = 1; i <= len2; ++i) d[0][i] = i;
//
//    for (size_t i = 1; i <= len1; ++i)
//        for (size_t j = 1; j <= len2; ++j)
//            d[i][j] = std::min({ d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) });
//    
//    return d[len1][len2];
//}
//
//std::vector<std::string> StringToString::removeString(std::string fileName)
//{
//    std::vector<std::string> str;
//
//    return str;
//}

bool StringToString::isMap(std::string str1, std::string str2) {
    std::vector<std::string> substringCounts1 = removeString(str1);
    std::vector<std::string> substringCounts2 = removeString(str2);
    if (substringCounts1 == substringCounts2)
        return true;
    return false;
}

bool StringToString::isMapCoutInformtion(std::string str1, std::string str2) {
    bool ret = false;

    std::vector<std::string> substringCounts1 = removeString(str1);
    for (auto& p : substringCounts1)
        std::cout << "str1: ( " << p << " )" << std::endl;

    std::cout << std::endl;

    std::vector<std::string> substringCounts2 = removeString(str2);
    for (auto& p : substringCounts2)
        std::cout << "str2: ( " << p << " )" << std::endl;

    std::cout << "----------------------------------------------------------------------------------------------\n";

    if (substringCounts1 == substringCounts2) {
        std::cout << "yes ( " << str1 << " | " << str2 << " )\n";
        ret = true;
    }
    else {
        std::cout << "no ( " << str1 << " | " << str2 << " )\n";
    }

    std::cout << "----------------------------------------------------------------------------------------------\n";
    std::cout << "----------------------------------------------------------------------------------------------\n";
    return ret;
}



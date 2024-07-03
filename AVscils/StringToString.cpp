#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <unordered_set>
#include <ranges>
#include "StringToString.h"
#include <regex>

// Function to check if a word should be included
bool StringToString::isIsclu(const std::wstring& word) {
    static std::unordered_set<std::wstring> excludeWords = { L"v", L"-" };
    return excludeWords.find(word) != excludeWords.end();
}

std::wstring StringToString::removeOnString(std::wstring& t_wstr) {
    std::wstring result;
    std::transform(t_wstr.begin(), t_wstr.end(), std::back_inserter(result), [](char c) {
        return std::tolower(c);
        });
    return result;
}

// Function to check string validity
bool checkString(const std::wstring& word, bool isSubstrW3x) {
    if (word.empty()) {
        return true;
    }
    if (isSubstrW3x) {
        return std::all_of(word.begin(), word.end(), ::isdigit);
    }
    return false;
}

// Function to process the filename and extract meaningful parts
std::vector<std::wstring> StringToString::removeString(const std::wstring& t_fileName) {
    std::wstring fileName = t_fileName;
    bool isSubstrW3x = false;
    // Remove file extension
    if (fileName.size() > 4 && fileName.substr(fileName.size() - 4) == L".w3x") {
        fileName.erase(fileName.size() - 4);
        isSubstrW3x = true;
    }

    // Replace specific characters with spaces
    std::replace(fileName.begin(), fileName.end(), '_', ' ');

    std::wstringstream ss(fileName);
    std::vector<std::wstring> strVector;
    std::wstring word;
    bool isStrRpg = false;

    while (ss >> word) {
        
        std::wstring transformedWord = removeOnString(word);
        if (isStrRpg) {
            std::wregex englishRegex(L"^[0-9]*$");
            bool oneNumber = std::regex_match(transformedWord, englishRegex);
            if(transformedWord.size() == 1 && oneNumber)
                strVector.push_back(transformedWord);
            else if(transformedWord.size() == 2 && transformedWord[0] == L's' && (transformedWord[1] >= L'0'  && transformedWord[1] <= L'9'))
                strVector.push_back(transformedWord);
            break;
        }
        if (transformedWord.find(L"rpg") != std::string::npos) {
            isStrRpg = true;
            size_t size = transformedWord.find(L"rpg");
            if(!(transformedWord[size + 3] >= L'0' && transformedWord[size + 3] <= L'9' && transformedWord.size() == size + 4))
                transformedWord = transformedWord.substr(0, size + 3);
        }
        strVector.push_back(transformedWord);
    }

    if (!isStrRpg) {//
        for (std::wstring& str : strVector) {
            bool isRezult = true;
            std::wstring str2 = str;


            std::wregex englishRegex(L"^[a-z0-9.]*$");
            std::wregex englishRegex1(L"^[0-9]*$");
            bool oneNumber = std::regex_match(str2, englishRegex1);
            if (str2.size() == 1 && oneNumber)
                continue;
            else if (str2.size() == 2 && str2[0] == L's' && (str2[1] >= L'0' && str2[1] <= L'9'))
                continue;
            else if (str2.size() == 8 && str2.substr(0, 7) == L"season" && (str2[7] >= L'0' && str2[7] <= L'9'))
                continue;

            if (std::regex_match(str2, englishRegex)) {
                int oneBucva = 0;
                for (const wchar_t& c : str2) {
                    if (c >= L'a' && c <= L'z') {
                        oneBucva++;
                    }
                    if (oneBucva >= 2) {
                        isRezult = false;
                        break;
                    }
                }
                if (isRezult) {
                    str = L"\0";
                }
                else
                {
                    for (int j = 0; j < str2.size(); j++) {

                        if (str2[j] >= '0' && str2[j] <= '9' && (str2[j+1] == '.') && str2[j+2] >= '0' && str2[j+2] <= '9') {
                            str = str2.substr(0, j);
                        }
                    }
                }
            }
        }
    }

    std::vector<std::wstring> strReturn;
    for (std::wstring& str : strVector) {
        if (!isIsclu(str) && !checkString(str, isSubstrW3x)) {
            strReturn.push_back(str);
        }
    }

    return strReturn;
}
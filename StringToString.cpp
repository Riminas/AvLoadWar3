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

// Function to normalize and transform specific strings
std::string StringToString::removeOnString(const std::string& t_str) {
    std::string result;
    std::transform(t_str.begin(), t_str.end(), std::back_inserter(result), [](char c) {
        return std::tolower(c);
        });
    return result;
}

// Function to check if a word should be included
bool StringToString::isIsclu(const std::string& word) {
    static std::unordered_set<std::string> excludeWords = { "v", "-" };
    return excludeWords.find(word) != excludeWords.end();
}

// Function to check string validity
bool checkString(const std::string& word, bool isSubstrW3x) {
    if (word.empty()) {
        return true;
    }
    if (isSubstrW3x) {
        return std::all_of(word.begin(), word.end(), ::isdigit);
    }
    return false;
}

// Function to process the filename and extract meaningful parts
std::vector<std::string> StringToString::removeString(const std::string& t_fileName) {
    std::string fileName = t_fileName;
    std::vector<std::string> strReturn;
    bool isSubstrW3x = false;

    // Remove file extension
    if (fileName.size() > 4 && fileName.substr(fileName.size() - 4) == ".w3x") {
        fileName.erase(fileName.size() - 4);
        isSubstrW3x = true;
    }

    // Replace specific characters with spaces
    std::replace(fileName.begin(), fileName.end(), '_', ' ');

    std::stringstream ss(fileName);
    std::vector<std::string> strVector;
    std::string word;
    bool isStrRpg = false;

    while (ss >> word) {
        std::string transformedWord = removeOnString(word);
        if (isStrRpg) {
            std::regex englishRegex("^[0-9]*$");
            bool oneNumber = std::regex_match(transformedWord, englishRegex);
            if(transformedWord.size() == 1 && oneNumber)
                strVector.push_back(transformedWord);
            else if(transformedWord.size() == 2 && transformedWord[0] == 's' && (transformedWord[1] >= '0'  && transformedWord[1] <= '9'))
                strVector.push_back(transformedWord);
            break;
        }
        if (transformedWord.find("rpg") != std::string::npos) {
            isStrRpg = true;
            size_t size = transformedWord.find("rpg");
            if(!(transformedWord[size + 3] >= '0' && transformedWord[size + 3] <= '9' && transformedWord.size() == size + 4))
                transformedWord = transformedWord.substr(0, size + 3);
        }
        strVector.push_back(transformedWord);
    }

    if (!isStrRpg) {//
        for (std::string& str : strVector) {
            bool isRezult = true;
            std::string str2 = str;


            std::regex englishRegex("^[a-z0-9.]*$");
            std::regex englishRegex1("^[0-9]*$");
            bool oneNumber = std::regex_match(str2, englishRegex1);
            if (str2.size() == 1 && oneNumber)
                continue;
            else if (str2.size() == 2 && str2[0] == 's' && (str2[1] >= '0' && str2[1] <= '9'))
                continue;
            else if (str2.size() == 8 && str2.substr(0, 7) == "season" && (str2[7] >= '0' && str2[7] <= '9'))
                continue;

            if (std::regex_match(str2, englishRegex)) {
                int oneBucva = 0;
                for (const char& c : str2) {
                    if (c >= 'a' && c <= 'z') {
                        oneBucva++;
                    }
                    if (oneBucva >= 2) {
                        isRezult = false;
                        break;
                    }
                }
                if (isRezult) {
                    str = "\0";
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

    for (std::string& str : strVector) {
        if (!isIsclu(str) && !checkString(str, isSubstrW3x)) {
            strReturn.push_back(str);
        }
    }

    return strReturn;
}


std::string StringToString::strVectorToStr(const std::string& t_fileName) {
    std::cout << "--------------------------------------------------\n";
    std::vector<std::string> strVector = removeString(t_fileName);
    if (strVector.size() == 0) {
        std::cout << "strVector.size(): ( " << strVector.size() << " )" << std::endl;
        return "\0";
    }
    std::string nameMaps = strVector[0];
    for (const auto& p : strVector | std::views::drop(1))
        nameMaps += '_' + p;
    nameMaps += ".txt";

    for (const auto& p : strVector)
        std::cout << "str: ( " << p << " )" << std::endl;
    std::cout << std::endl;

    return nameMaps;
}

// Function to test the comparison with given test cases
void StringToString::testStringComparison(const std::vector<std::pair<std::pair<std::string, std::string>, bool>>& testCases) {
    for (const auto& testCase : testCases) {
        const auto& strings = testCase.first;
        bool expected = testCase.second;

        std::vector<std::string> substringCounts1 = removeString(strings.first);
        std::vector<std::string> substringCounts2 = removeString(strings.second);

        bool result = substringCounts1 == substringCounts2;

        std::cout << "Comparing: \"" << strings.first << "\" and \"" << strings.second << "\"\n";
        std::cout << "Expected: " << std::boolalpha << expected << ", Got: " << result << "\n";
        if (result == expected) {
            std::cout << "Test passed.\n";
        }
        else {
            std::cout << "Test failed.\n";
            for (const auto& p : substringCounts1)
                std::cout << "str1: ( " << p << " )" << std::endl;
            std::cout << std::endl;
            for (const auto& p : substringCounts2)
                std::cout << "str2: ( " << p << " )" << std::endl;
        }
        std::cout << "--------------------------------------------------\n";
        std::cout << std::endl;
    }
}
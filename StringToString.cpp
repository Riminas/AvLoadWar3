#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <unordered_set>
#include "StringToString.h"

// Function to normalize and transform specific strings
std::string StringToString::removeOnString(const std::string& t_str) {
    std::string str;
    std::transform(t_str.begin(), t_str.end(), std::back_inserter(str), ::tolower);

    if (str == "zm666") return "zombie666";

    if (str.substr(0, 3) == "eng") return "eng";
    if (str.substr(0, 5) == "30min") return "30min";
    if (str.substr(0, 6) == "disrpg") return "disrpg";
    if (str.substr(0, 9) == "animalrpg") return "animalrpg";

    return str;
}

// Function to check if a word should be included
bool StringToString::isIsclu(const std::string& word) {
    std::string str;
    std::transform(word.begin(), word.end(), std::back_inserter(str), ::tolower);

    if (str.size() > 2 && str[0] == 'v' && std::isdigit(str[1])) return true;

    static const std::unordered_set<std::string> includedWords = {
        "eng", "bug", "hotfix", "jn", "world", "worl", "raid", "rai", "final", "nal",
        "test", "rus", "ru", "translated", "ranslated"
    };

    if (includedWords.find(str) != includedWords.end()) return true;
    if (str.substr(0, 3) == "fix") return true;
    if (str.substr(0, 13) == "translatedfix") return true;

    return false;
}

// Function to process the filename and extract meaningful parts
std::vector<std::string> StringToString::removeString(const std::string& t_fileName) {
    std::string fileName = t_fileName;
    std::unordered_map<std::string, int> mapString;

    // Replace specific characters with spaces
    std::replace_if(fileName.begin(), fileName.end(),
        [](char c) { return c == '_' || c == '-' || c == '(' || c == ')' || c == '[' || c == ']'; }, ' ');

    // Remove file extension
    if (fileName.size() > 4 && fileName.substr(fileName.size() - 4) == ".w3x") {
        fileName.erase(fileName.size() - 4);
    }

    std::stringstream ss(fileName);
    std::string word;
    while (ss >> word) {
        bool isOneNumber = std::any_of(word.begin(), word.end(), ::isdigit);

        if (!isIsclu(word) && !isOneNumber) {
            std::string str2 = removeOnString(word);
            if (!isIsclu(str2)) {
                mapString[str2]++;
            }
        }
    }

    std::vector<std::string> strReturn;
    for (const auto& p : mapString) {
        strReturn.push_back(p.first);
    }

    return strReturn;
}

// Function to compare two strings after processing
bool StringToString::isMap(const std::string& str1, const std::string& str2) {
    return removeString(str1) == removeString(str2);
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
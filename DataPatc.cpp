#include <iostream>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <unordered_map>
#include "StringToString.h"
#include "DataPatc.h"
#include <Windows.h>
#include <ShlObj_core.h>

bool DataPatc::loadDataOptions()
{
	PWSTR path = NULL;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);
	if (SUCCEEDED(hr)) {
		std::string str = "\0";
		{
			const std::wstring wStr = path;
			str = { wStr.begin(), wStr.end() };
			str += +"\\Warcraft III";
		}

		CoTaskMemFree(path);
		if (std::filesystem::is_directory((str).c_str())) {

			m_Maps = str + "\\maps";
			std::cout << "putWarcraftMaps ( " << m_Maps << " )" << std::endl;

			m_Save = str + "\\CustomMapData";
			std::cout << "putWarcraftSave ( " << m_Save << " )" << std::endl;

			return true;
		}
		else {
			std::cout << "Error: directory Warcraft III(" << str << ")" << std::endl;
			return false;
		}
	}
	else {
		std::cout << "Error: SHGetKnownFolderPath()" << std::endl;
		return false;
	}

	return false;
}

std::vector<DataPatc::listHero> DataPatc::getFileSaveCode1(const std::string& t_PutSaveCode, bool& isError)
{
    std::string strPut = m_Save + t_PutSaveCode;

    if (!std::filesystem::is_directory(strPut.c_str())) {
        std::cout << "Error: directory - (" << strPut << ")" << std::endl;
        isError = true;
        return m_ListHero1;
    }

     m_ListHero1 = lastFail(strPut);
     return m_ListHero1;
}


std::vector<DataPatc::listHero> DataPatc::lastFail(const std::string& folder_path) {
    std::unordered_map<std::string, listHero> mapData;

    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_directory()) {
            listHero &listHeroMap = mapData[entry.path().filename().string()];
            
            for (const auto& entry2 : std::filesystem::directory_iterator(entry.path())) {
                if (std::filesystem::is_regular_file(entry2) && entry2.path().extension() == ".txt") {
                    std::filesystem::file_time_type fileTime = entry2.last_write_time();
                    if (fileTime > listHeroMap.latestTime) {
                        listHeroMap.latestTime = fileTime;
                        listHeroMap.patc = entry2.path().string();
                    }
                }
            }
		}
		else
		{
            if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".txt") {
                std::filesystem::file_time_type fileTime = entry.last_write_time();
               
                std::ifstream fil(entry.path());

                std::string input_str;
                bool isEnd = false;
                while (fil) {
                    std::getline(fil, input_str);

                    for (char& c : input_str) {
                        if (c == '(' || c == ')' || c == ':' || c == '"')
                            c = ' ';
                    }

                    std::istringstream iss(input_str);
                    std::vector<std::string> strVector(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

                    int i = 0;
                    for (std::string& str : strVector) {
                        std::string str2 = "\0";
                        for (const char& c : str) {
                            str2 += std::tolower(c);
                        }
                        if (str2 == "hero" || str2 == "job" || str2 == "char" || str2 == "Character" || str2 == "перс") {
                            i += 1;
                            if (strVector[i] == "name")
                                i += 1;
                            str2 = strVector[i];
                            for (int j = i + 1; j < strVector.size(); j++) {
                                str2 += " " + strVector[j];
                            }

                            listHero& listHeroMap = mapData[str2];
                            if (fileTime > listHeroMap.latestTime) {
                                listHeroMap.latestTime = fileTime;
                                listHeroMap.patc = entry.path().string();
                            }

                            isEnd = true;
                            break;
                        }
                        i++;
                    }
                    if (isEnd)
                        break;
                }

                fil.close();
            }
           /* if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".txt") {
                std::filesystem::file_time_type fileTime = entry.last_write_time();
                if (fileTime > listHeroMap.latestTime) {
                    listHeroMap.latestTime = fileTime;
                    listHeroMap.patc = entry.path().string();
                }
            }*/
		}
    }

    std::vector<listHero> listHero1;
    for (auto& p : mapData) {
        listHero1.push_back(listHero(p.first, p.second.patc, p.second.latestTime));
        if (listHero1.size() > 9)
            break;
    }
    return listHero1;
}

//std::string DataPatc::lastFail(const std::string& t_PutSaveCode) const {
//    std::filesystem::file_time_type latestTime = std::filesystem::file_time_type();
//    std::string latestFile;
//
//    const std::string patcPutSaveCode = m_Save + t_PutSaveCode;
//
//    if (std::filesystem::is_directory(patcPutSaveCode.c_str())) {
//
//        for (const auto& entry : std::filesystem::recursive_directory_iterator(patcPutSaveCode)) {
//            if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".txt") {
//                std::filesystem::file_time_type fileTime = entry.last_write_time();
//                if (fileTime > latestTime) {
//                    latestTime = fileTime;
//                    latestFile = entry.path().string();
//                }
//            }
//        }
//    }
//
//    std::cout << "Loads: (" << latestFile << ")" << std::endl;
//
//    return latestFile;
//}

std::string DataPatc::getMaps() { return m_Maps; }
std::string DataPatc::getSave() { return m_Save; }
std::vector<DataPatc::listHero> DataPatc::getListHero1() { return m_ListHero1; }

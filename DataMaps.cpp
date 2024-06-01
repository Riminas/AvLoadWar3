#include <vector>
#include <ranges>
#include <iostream>
#include <fstream>
#include "StringToString.h"
#include "DataMaps.h"


bool DataMaps::loadDataMaps(const std::string& fileName) {
    NameMaps(fileName);

    return PutSaveCode();
}

void DataMaps::NameMaps(const std::string& fileName) {
    StringToString StringToString_;
    std::vector<std::string> nameFileRemove = StringToString_.removeString(fileName);

    m_NameMaps = nameFileRemove[0];
    for (std::string& p : nameFileRemove | std::views::drop(1))
        m_NameMaps += '_' + p;
    m_NameMaps += ".txt";
}

bool DataMaps::PutSaveCode() {
    if (loadDatFail()) {
        std::cout << "Loads: " << m_PutSaveCode << std::endl;
        return true;
    }

    std::cout << "Error: Not information file ( " << m_putDatMapPut << "\\" << m_NameMaps << " )" << std::endl;
    std::cout << "create file ( " << m_NameMaps << " ) and save code directory" << std::endl;
    std::string str = "notepad.exe " + m_putDatMapPut + "\\" + m_NameMaps;
    system(str.c_str());
    return false;
}

bool DataMaps::loadDatFail() {
    std::ifstream fil((m_putDatMapPut + '\\' + m_NameMaps).c_str());
    if (!fil.is_open())
        return false;

    std::getline(fil, m_PutSaveCode);
    return true;
}

std::string DataMaps::getPutSaveCode() { return m_PutSaveCode; }
std::string DataMaps::getNameMaps() { return m_NameMaps; }
std::string DataMaps::getPutDatMapPut() { return m_putDatMapPut; }

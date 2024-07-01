#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <cctype>
#include <Windows.h>

#include "SkillsUpgradeStart.h"
#include "key.h"

SkillsUpgradeStart::SkillsUpgradeStart(const std::wstring& t_NameMaps)
	: m_NameMaps(t_NameMaps)
{
}

void SkillsUpgradeStart::skillsUpgradeStart()
{
	m_FullPath = m_pathDatMap + L'\\' + m_NameMaps + m_pathFile;
	if (!std::filesystem::exists(m_FullPath))
		return;

	Sleep(5000);

	if (!loadDataSkill())
		return;

	for (std::pair<int, unsigned>& p : m_DataSkill)
		if (!charToAscii(p.first))
			return;

	key key_;
	key_.key1(112);
	Sleep(100);
	key_.key1(112);
	Sleep(100);
	key_.key1(79);

	Sleep(100);

	for (const std::pair<int, unsigned>& p : m_DataSkill) {
		for (unsigned i = 0; i < p.second; i++) {
			key_.key1(p.first);
			Sleep(10);
		}
	}

	Sleep(100);
	key_.key1(27);

}

bool SkillsUpgradeStart::loadDataSkill()
{
	std::ifstream inFile(m_FullPath);
	
	std::string line;
	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
		std::vector<std::string> tokens(std::istream_iterator<std::string, char>{iss}, std::istream_iterator<std::string, char>());

		if (tokens.size() != 2)
			return false;

		int idKey = 0;
		{
			if(tokens[0].size() != 1)
				return false;

			idKey = tokens[0][0];
		}

		unsigned numKey = 0;
		{
			try {
				numKey = std::stoul(tokens[1]);
			}
			catch (const std::invalid_argument& e) {
				// Обработка ошибки: строка не является числом
				std::cerr << "Invalid argument: " << e.what() << std::endl;
			}
			catch (const std::out_of_range& e) {
				// Обработка ошибки: число выходит за пределы типа unsigned int
				std::cerr << "Out of range: " << e.what() << std::endl;
			}
		}

		const std::pair<int, unsigned> pair{ idKey, numKey };
		m_DataSkill.push_back(pair);
	}

	return true;
}

bool SkillsUpgradeStart::charToAscii(int& c) {
	// Преобразование символа в верхний регистр
	char upperC = std::toupper(static_cast<unsigned char>(c));

	// Проверка, является ли символ буквой
	if (std::isalpha(static_cast<unsigned char>(upperC))) {
		c = static_cast<int>(upperC);
		return true;
	}
	return false; // Возвращаем пустое значение для небуквенных символов
}

//int SkillsUpgradeStart::charToAscii(char c) {
//	// Проверка, является ли символ буквой
//	if (std::isalpha(static_cast<unsigned char>(c))) {
//		return static_cast<int>(c);
//	}
//	return с; // Возвращаем пустое значение для небуквенных символов
//}
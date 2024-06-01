#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "load.h"
#include "key.h"
#include <stdio.h>
#include <filesystem>
#include <ctime>
#include <algorithm>
#include <map>
#include <future>

load::load() {
	m_targetWindow = FindWindow(NULL, L"Warcraft III"); // «амените "»м€ окна" на им€ окна, которому нужно отправить сообщение
}

bool IsEqualKL(HKL hKL, WORD wLanguage) {
	const BYTE* pb = (const BYTE*)hKL;
	return (pb[0] == 0) && (pb[2] == wLanguage);
}

bool load::load1(const std::string& put) {
	m_Put = put;

	key key_;

	std::vector<std::string> codeAll(10, "\0");

	const bool isCodeAllProv = codeAllProv(codeAll);

	if (isCodeAllProv)
	{
		SetForegroundWindow(m_targetWindow);
		Sleep(100);


		if (m_TipLoad == 1) {
			PostMessage(m_targetWindow, WM_INPUTLANGCHANGEREQUEST, 1, 0x04090409);//английска€ раскладка
			Sleep(100);

			coutCommands("-load");

		}

		for (std::string& code : codeAll) {
			if (code == "\0")
				break;

			Sleep(500);

			SetForegroundWindow(m_targetWindow);
			Sleep(100);


			while (GetAsyncKeyState(16) & 0x8000 || GetAsyncKeyState(17) & 0x8000 || GetAsyncKeyState(18) & 0x8000) { Sleep(100); }

			PostMessage(m_targetWindow, WM_INPUTLANGCHANGEREQUEST, 1, 0x04090409);//английска€ раскладка
			Sleep(200);

			key_.key1(0x0D);//entr открыть чат

			convectorCharKey(code);

			key_.key1(0x0D);//entr закрыть чат

			Sleep(10);
			std::cout << "codes: (" << code << ")" << std::endl;
		}
	}
	Sleep(100);
	PostMessage(m_targetWindow, WM_INPUTLANGCHANGEREQUEST, 1, 0x4190419);//–усска€ раскладка

	return isCodeAllProv;
}

bool load::codeAllProv(std::vector<std::string>& codeAll) {


	std::ifstream fil(m_Put);
	if (!fil.is_open())
		return false;

	std::string str;
	while (std::getline(fil, str)) {
		std::istringstream iss(str);
		std::vector<std::string> sts(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

		for (size_t u = 0; u < sts.size(); u++) {
			if (sts[u] == "\0") {
				break;
			}
			else if (sts[u].size() == 7) {
				if (sts[u] == "\"Code1:"/* || sts[u] == "\"Code1" || sts[u] == "Code1:" || sts[u] == "Code1"*/) {
					m_TipLoad = 1;
					provOne(codeAll[0], sts, (short)u + 1);
				}
				else if (sts[u] == "\"Code2:"/* || sts[u] == "\"Code2" || sts[u] == "Code2:" || sts[u] == "Code2"*/)
					provOne(codeAll[1], sts, (short)u + 1);
			}
			else if (sts[u].size() == 6) {
				if (sts[u] == "\"-load") {
					provOne(codeAll[0], sts, (short)(u + 1), "-load ");
					break;
				}
				else if (sts[u] == "-Load1")
					provOne(codeAll[1], sts, (short)(u + 1), "-Load1 ");
				else if (sts[u] == "-Load2")
					provOne(codeAll[0], sts, (short)(u + 1), "-Load2 ");
			}
			else if (sts[u] == "-load") {
				provOne(codeAll[0], sts, (short)(u + 1), "-load ");
				break;
			}
			else if (sts[u] == "Preload(" && sts[u + 1].size() >= 25 && sts[u + 2] == ")") {
				provOne(codeAll[0], sts, (short)(u + 1), "-load ");
				break;
			}
		}
	}
	fil.close();
	return true;
}

void load::provOne(std::string& code, const std::vector<std::string> sts, short iu, std::string addPref) {
	short stsSize = (short)sts.size();
	if (stsSize > iu) {
		do
		{
			if (sts[iu] != ":") {
				code = sts[iu];
				break;
			}
			else {
				iu++;
			}
		} while (true);

	}

	short stsIuSize = (short)sts[iu].size() - 1;
	if (sts[iu][stsIuSize] == '"' && sts[iu + 1] == ")") {// CODE" )
		code.erase(stsIuSize, 1);
		if (sts[iu][0] == '"')
			code.erase(0, 1);
	}
	else if (sts[iu][stsIuSize - 1] == '"' && sts[iu][stsIuSize] == ')' && stsSize == iu + 1) {// CODE")
		code.erase(stsIuSize - 1, 2);
	}

	if (addPref != "\0") //добовл€ем префикс тип загрузки -load -Load1 -Load2
		code.insert(0, addPref);

}

void load::convectorCharKey(const std::string& code) {
	key key_;
	for (int i = 0; i < code.size(); i++) {
		if ((int)code[i] >= 97 && (int)code[i] <= 122) {
			key_.key1((int)code[i] - 32);
		}
		else if ((int)code[i] >= 65 && (int)code[i] <= 90) {
			key_.key2((int)code[i]);
		}
		else if ((int)code[i] >= 48 && (int)code[i] <= 57) {
			key_.key1((int)code[i]);
		}
		else {
			switch ((int)code[i])
			{
			case 32://Space
				key_.key1(32);
				break;
			case 33://!
				key_.key2(49);
				break;
			case 34://"
				key_.key2(222);
				break;
			case 35://#
				key_.key2(51);
				break;
			case 36://$
				key_.key2(52);
				break;
			case 37://%
				key_.key2(53);
				break;
			case 38://&
				key_.key2(55);
				break;
			case 39://'
				key_.key1(222);
				break;
			case 40://(
				key_.key2(57);
				break;
			case 41://)
				key_.key2(48);
				break;
			case 42://*
				key_.key2(56);
				break;
			case 43://+
				key_.key2(187);
				break;
			case 44://,
				key_.key1(188);
				break;
			case 45://-
				key_.key1(189);
				break;
			case 46://.
				key_.key1(190);
				break;
			case 47:///
				key_.key1(191);
				break;
			case 58://:
				key_.key2(186);
				break;
			case 59://;
				key_.key1(186);
				break;
			case 60://<
				key_.key2(188);
				break;
			case 61://=
				key_.key1(187);
				break;
			case 62://>
				key_.key2(190);
				break;
			case 63://?
				key_.key2(191);
				break;
			case 64://@
				key_.key2(50);
				break;
			case 91://[
				key_.key1(219);
				break;
			case 92://\ 
				key_.key1(229);
				break;
			case 93://]
				key_.key1(221);
				break;
			case 94://^
				key_.key2(54);
				break;
			case 95://_
				key_.key2(189);
				break;
			case 96://`
				key_.key1(192);
				break;
			case 123://{
				key_.key2(219);
				break;
			case 124://|
				key_.key2(229);
				break;
			case 125://}
				key_.key2(221);
				break;
			case 126://~
				key_.key2(192);
				break;
			default:
				std::cout << "ERROR char( " << code[i] << " )" << std::endl;
				break;
			}
		}
	}
}

void load::coutCommands(const std::string& strComand)
{
	std::cout << "Cmand: " << strComand.c_str() << " " << std::endl;
	Sleep(2);

	key key_;
	key_.key1(0x0D);//entr открыть чат

	Sleep(2);

	convectorCharKey(strComand);
	Sleep(2);

	key_.key1(0x0D);//entr закрыть чат

	Sleep(2);

}

#include <locale>
#include <fstream>
#include <iostream>
#include <wtypes.h>
#include <windows.h>

#include "Engine.h"

int main()
{//mainCRTStartup
    SetConsoleOutputCP(CP_UTF8); // Устанавливаем кодировку консоли на UTF-8

    std::locale::global(std::locale("Russian"));
    std::wcout.imbue(std::locale());

    HWND hWndWindow = FindWindow(NULL, L"AvLoads");
    if (hWndWindow == NULL) {
        Engine Engine_;
        Engine_.engine1();
    }
    else {
        MessageBox(NULL, L"Программа уже запущена.", L"Ошибка", MB_OK | MB_ICONEXCLAMATION);
    }

    return 0;
}

//#include <iostream>
//#include <tesseract/baseapi.h>
//#include <leptonica/allheaders.h>
//
//int main() {
//    // Инициализация Tesseract API
//    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
//
//    // Устанавливаем язык (например, английский)
//    if (api->Init(NULL, "eng")) {
//        std::cerr << "Could not initialize tesseract." << std::endl;
//        return 1;
//    }
//
//    // Загружаем изображение
//    Pix* image = pixRead("image.png");
//    if (!image) {
//        std::cerr << "Could not open input image." << std::endl;
//        return 1;
//    }
//
//    // Выполняем OCR
//    api->SetImage(image);
//    char* outText = api->GetUTF8Text();
//
//    // Выводим распознанный текст
//    std::cout << "OCR output:\n" << outText << std::endl;
//
//    // Освобождаем ресурсы
//    api->End();
//    delete[] outText;
//    pixDestroy(&image);
//
//    return 0;
//}





//#include <iostream>
//#include <optional>
//#include <cctype> // для std::isalpha
//#include <Windows.h>
//
//std::optional<int> charToAscii(char c) {
//    // Преобразование символа в верхний регистр
//    char upperC = std::toupper(static_cast<unsigned char>(c));
//
//    // Проверка, является ли символ буквой
//    if (std::isalpha(static_cast<unsigned char>(upperC))) {
//        return static_cast<int>(upperC);
//    }
//    return std::nullopt; // Возвращаем пустое значение для небуквенных символов
//}
//
//int main() {
//        SetConsoleOutputCP(CP_UTF8); // Устанавливаем кодировку консоли на UTF-8
//    
//        std::locale::global(std::locale("Russian"));
//        std::wcout.imbue(std::locale());
//    char input;
//    std::cout << "Введите символ: ";
//    std::cin >> input;
//
//    auto result = charToAscii(input);
//    if (result) {
//        std::cout << "ASCII код буквы '" << input << "' равен " << *result << std::endl;
//    }
//    else {
//        std::cout << "Введенный символ не является буквой." << std::endl;
//    }
//
//    return 0;
//}
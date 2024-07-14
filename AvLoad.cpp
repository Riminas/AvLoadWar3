#include <locale>
#include <fstream>
#include <iostream>
#include <wtypes.h>
#include <windows.h>

#include "Engine.h"


int main()
{
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


//
////#include <iostream>
////#include <tesseract/baseapi.h>
////#include <leptonica/allheaders.h>
////
////int main() {
////    // Инициализация Tesseract API
////    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
////
////    // Устанавливаем язык (например, английский)
////    if (api->Init(NULL, "eng")) {
////        std::cerr << "Could not initialize tesseract." << std::endl;
////        return 1;
////    }
////
////    // Загружаем изображение
////    Pix* image = pixRead("image.png");
////    if (!image) {
////        std::cerr << "Could not open input image." << std::endl;
////        return 1;
////    }
////
////    // Выполняем OCR
////    api->SetImage(image);
////    char* outText = api->GetUTF8Text();
////
////    // Выводим распознанный текст
////    std::cout << "OCR output:\n" << outText << std::endl;
////
////    // Освобождаем ресурсы
////    api->End();
////    delete[] outText;
////    pixDestroy(&image);
////
////    return 0;
////}
//
//
//
//
//
////#include <iostream>
////#include <optional>
////#include <cctype> // для std::isalpha
////#include <Windows.h>
////
////std::optional<int> charToAscii(char c) {
////    // Преобразование символа в верхний регистр
////    char upperC = std::toupper(static_cast<unsigned char>(c));
////
////    // Проверка, является ли символ буквой
////    if (std::isalpha(static_cast<unsigned char>(upperC))) {
////        return static_cast<int>(upperC);
////    }
////    return std::nullopt; // Возвращаем пустое значение для небуквенных символов
////}
////
////int main() {
////        SetConsoleOutputCP(CP_UTF8); // Устанавливаем кодировку консоли на UTF-8
////    
////        std::locale::global(std::locale("Russian"));
////        std::wcout.imbue(std::locale());
////    char input;
////    std::cout << "Введите символ: ";
////    std::cin >> input;
////
////    auto result = charToAscii(input);
////    if (result) {
////        std::cout << "ASCII код буквы '" << input << "' равен " << *result << std::endl;
////    }
////    else {
////        std::cout << "Введенный символ не является буквой." << std::endl;
////    }
////
////    return 0;
////}



//#include <opencv2/opencv.hpp>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//bool findRedCross(const Mat& image) {
//    // Определим регион интереса (ROI) в правом нижнем углу
//    int roiWidth = 200; // ширина региона интереса
//    int roiHeight = 200; // высота региона интереса
//    Rect roi(image.cols - roiWidth, image.rows - roiHeight, roiWidth, roiHeight);
//
//    Mat croppedImage = image(roi);
//
//    // Преобразуем изображение в цветовое пространство HSV
//    Mat hsvImage;
//    cvtColor(croppedImage, hsvImage, COLOR_BGR2HSV);
//
//    // Определим границы для красного цвета в HSV пространстве
//    Scalar lowerRed1(0, 100, 100);
//    Scalar upperRed1(10, 255, 255);
//    Scalar lowerRed2(160, 100, 100);
//    Scalar upperRed2(179, 255, 255);
//
//    Mat mask1, mask2, redMask;
//    inRange(hsvImage, lowerRed1, upperRed1, mask1);
//    inRange(hsvImage, lowerRed2, upperRed2, mask2);
//
//    // Объединяем обе маски
//    bitwise_or(mask1, mask2, redMask);
//
//    // Ищем контуры на маске
//    vector<vector<Point>> contours;
//    findContours(redMask, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
//
//    for (const auto& contour : contours) {
//        double area = contourArea(contour);
//        if (area > 100) { // Порог площади для фильтрации мелких объектов
//            // Предполагаем, что если есть достаточно большой красный объект, это красный крестик
//            return true;
//        }
//    }
//
//    return false;
//}
//
//int main() {
//    // Загрузим изображение
//    Mat image = imread("image2.png");
//
//    if (image.empty()) {
//        cerr << "Image not found!" << endl;
//        return -1;
//    }
//
//    // Проверим наличие красного крестика
//    bool found = findRedCross(image);
//
//    if (found) {
//        cout << "Red cross found!" << endl;
//    }
//    else {
//        cout << "Red cross not found." << endl;
//    }
//
//    return 0;
//}
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include "CoutButtonUser.h"

// Функция для захвата экрана
cv::Mat CoutButtonUser::captureScreen() {
    // Определяем размеры экрана
    int screen_x = GetSystemMetrics(SM_CXSCREEN);
    int screen_y = GetSystemMetrics(SM_CYSCREEN);

    // Захватываем экран
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screen_x, screen_y);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

    BitBlt(hMemoryDC, 0, 0, screen_x, screen_y, hScreenDC, 0, 0, SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = screen_x;
    bi.biHeight = -screen_y;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    cv::Mat mat(screen_y, screen_x, CV_8UC4);
    GetDIBits(hMemoryDC, hBitmap, 0, screen_y, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // Освобождаем ресурсы
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
    DeleteObject(hBitmap);

    return mat;
}

sf::Vector2f CoutButtonUser::findElementInImage(const std::string& templatePath) {

    // Захватываем изображение экрана
    cv::Mat image = captureScreen();

    cv::Mat templ = cv::imread(templatePath, cv::IMREAD_COLOR);

    if (templ.empty()) {
        std::cerr << "Не удалось открыть шаблон изображения!" << std::endl;
        return  sf::Vector2f(-1, -1); // Возвращаем (-1, -1) если шаблон не найден
    }

    cv::Mat result;
    int result_cols = image.cols - templ.cols + 1;
    int result_rows = image.rows - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    cv::matchTemplate(image, templ, result, cv::TM_CCOEFF_NORMED);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

    double threshold = 0.8;
    if (maxVal >= threshold) {
        sf::Vector2f f = sf::Vector2f(maxLoc.x, maxLoc.y);
        return f;
    }
    else {
        return sf::Vector2f(-1, -1); // Возвращаем (-1, -1) если элемент не найден
    }
}

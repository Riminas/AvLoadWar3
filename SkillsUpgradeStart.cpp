#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <cctype>
#include <Windows.h>
#include <gdiplus.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "SkillsUpgradeStart.h"
#include "key.h"

using namespace cv;

#pragma comment (lib, "Gdiplus.lib")

SkillsUpgradeStart::SkillsUpgradeStart(const std::wstring& t_NameMaps, const int& t_VersionWarcraft3)
    : m_NameMaps(t_NameMaps),
    m_VersionWarcraft3(t_VersionWarcraft3)
{
}

cv::Mat SkillsUpgradeStart::captureScreen() {
    HWND hwnd = GetDesktopWindow();
    HDC hwindowDC = GetDC(hwnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize;
    GetClientRect(hwnd, &windowsize);

    int srcheight = windowsize.bottom;
    int srcwidth = windowsize.right;
    int height = srcheight;
    int width = srcwidth;

    Mat src(height, width, CV_8UC4);

    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    BITMAPINFOHEADER bi = { sizeof(BITMAPINFOHEADER), width, -height, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };

    SelectObject(hwindowCompatibleDC, hbwindow);
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY);
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}

void mouseClick(int x, int y) {
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
    Sleep(1);
    mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
}

void SkillsUpgradeStart::delayIsEndLoad() {
    int i = 100;
    Mat screen;
    bool iconFound = false;
    do {
        Sleep(100);  // небольшая задержка перед каждым новым скриншотом
        screen = captureScreen();
        Mat roi = screen(Rect(0, 0, 100, 100));  // область в левом верхнем углу

        Mat gray;
        cvtColor(roi, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, gray, Size(5, 5), 0);

        Mat edges;
        Canny(gray, edges, 50, 150);

        std::vector<std::vector<Point>> contours;
        findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours) {
            double area = contourArea(contour);
            if (area > 500) {  // порог для определения выделяющегося элемента
                iconFound = true;
                break;
            }
        }
        i--;
    } while (!iconFound && i >= 0);
}

bool areImagesSimilar(const Mat& img1, const Mat& img2, double blackThreshold = 0.95) {
    // Конвертируем изображения в градации серого
    Mat gray1, gray2;
    cv::cvtColor(img1, gray1, COLOR_BGR2GRAY);
    cv::cvtColor(img2, gray2, COLOR_BGR2GRAY);

    // Считаем количество черных пикселей
    int blackPixels1 = countNonZero(gray1 < 50);
    int blackPixels2 = countNonZero(gray2 < 50);
    int totalPixels = gray1.total();

    // Вычисляем процент черных пикселей
    double blackPercentage1 = static_cast<double>(blackPixels1) / totalPixels;
    double blackPercentage2 = static_cast<double>(blackPixels2) / totalPixels;

    // Проверяем, превышает ли процент черных пикселей пороговое значение
    return (blackPercentage1 > blackThreshold) && (blackPercentage2 > blackThreshold);
}

void SkillsUpgradeStart::skillsUpgradeStart(const bool& isDelay) {
    bool isExistsFullPath = false;
    m_FullPath = m_pathDatMap + L'\\' + m_NameMaps + m_pathFile;
    if (std::filesystem::exists(m_FullPath)) {
        isExistsFullPath = true;

        if (!loadDataSkill())
            isExistsFullPath = false;

        for (std::pair<int, unsigned>& p : m_DataSkill)
            if (!charToAscii(p.first)) {
                isExistsFullPath = false;
                break;
            }
    }

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    if(isDelay)
        delayIsEndLoad();

    key key_;
    key_.key1(112);
    Sleep(100);
    key_.key1(112);
    Sleep(100);

    Mat screen1 = captureScreen();

    key_.key1(70);
    Sleep(100);
    key_.key1(79);
    Sleep(100);

    if (isExistsFullPath) { // ручая настройка прокачи спелов
        for (const std::pair<int, unsigned>& p : m_DataSkill) {
            for (unsigned i = 0; i < p.second; i++) {
                key_.key1(p.first);
                Sleep(10);
            }
        }

        Sleep(100);
        key_.key1(27);
        return;
    }
    Mat screen2 = captureScreen();

    int roiWidth = screen1.cols / 4;
    int roiHeight = screen1.rows / 4;
    int roiShiftcircleX = 0;
    int roiShiftcircleY = 0;
    int roiShift = 0;
    int roiWidthReduction = 0;

    int m_VersionWarcraft3 = 1;
    if (m_VersionWarcraft3 == 1) {
        roiShift = -100;
        roiWidthReduction = 80;
        roiShiftcircleX = 25;
        roiShiftcircleY = 10;
    }

    Rect roi(screen1.cols - roiWidth + roiShift, screen1.rows - roiHeight, roiWidth - roiWidthReduction, roiHeight);
    Mat roi1 = screen1(roi);
    Mat roi2 = screen2(roi);

    int circleRadius = 50;
    Point circleCenter((roiWidth - circleRadius - 10 - roiWidthReduction) + roiShiftcircleX,
        (roiHeight - circleRadius + 20) + roiShiftcircleY);

    cv::circle(roi1, circleCenter, circleRadius, Scalar(0, 0, 0), -1);
    cv::circle(roi2, circleCenter, circleRadius, Scalar(0, 0, 0), -1);

    Mat gray1, gray2;
    cv::cvtColor(roi1, gray1, COLOR_BGR2GRAY);
    cv::cvtColor(roi2, gray2, COLOR_BGR2GRAY);

    Mat diff, thresh;
    cv::absdiff(gray1, gray2, diff);
    cv::threshold(diff, thresh, 25, 255, THRESH_BINARY);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    cv::dilate(thresh, thresh, kernel);
    cv::erode(thresh, thresh, kernel);

    std::vector<std::vector<Point>> contours;
    cv::findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Цикл для проверки схожести областей
    std::vector<std::vector<Point>> filteredContours;
    for (const auto& contour : contours) {
        // Проверка, является ли контур черным квадратом
        Rect boundingBox = boundingRect(contour);
        Mat mask = Mat::zeros(thresh.size(), CV_8UC1);
        drawContours(mask, std::vector<std::vector<Point>>{contour}, -1, Scalar(255), -1);

        // Получение среднего значения в области маски
        Scalar meanVal = cv::mean(roi2, mask);
        bool isBlackSquare = meanVal[0] < 5; // Порог для определения "черного"

        if (!isBlackSquare) {
            filteredContours.push_back(contour);
        }
    }

    // Основной цикл с имитацией нажатия мыши
    for (const auto& contour : filteredContours) {
        Rect boundingBox = boundingRect(contour);
        int centerX = boundingBox.x + boundingBox.width / 2 + screen1.cols - roiWidth + roiShift;
        int centerY = boundingBox.y + boundingBox.height / 2 + screen1.rows - roiHeight;
        for (int i = 0; i < 25; ++i) {
            mouseClick(centerX, centerY);
            Sleep(2);
        }
    }




    /*Mat previousScreen = screen2;

    for (int i = 0; i < 5; ++i) {
        Sleep(2000);
        Mat newScreen = captureScreen();
        Mat newRoi = newScreen(roi);
        circle(newRoi, circleCenter, circleRadius, Scalar(0, 0, 0), -1);
        Mat newGray;
        cvtColor(newRoi, newGray, COLOR_BGR2GRAY);

        Mat newDiff, newThresh;
        absdiff(gray1, newGray, newDiff);
        threshold(newDiff, newThresh, 25, 255, THRESH_BINARY);
        dilate(newThresh, newThresh, kernel);
        erode(newThresh, newThresh, kernel);

        std::vector<std::vector<Point>> newContours;
        findContours(newThresh, newContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (auto it = newContours.begin(); it != newContours.end(); ) {
            Rect boundingBox = boundingRect(*it);
            int newCenterX = boundingBox.x + boundingBox.width / 2 + screen1.cols - roiWidth;
            int newCenterY = boundingBox.y + boundingBox.height / 2 + screen1.rows - roiHeight;

            bool isNewChange = true;
            for (const auto& contour : contours) {
                Rect oldBoundingBox = boundingRect(contour);
                int oldCenterX = oldBoundingBox.x + oldBoundingBox.width / 2 + screen1.cols - roiWidth;
                int oldCenterY = oldBoundingBox.y + oldBoundingBox.height / 2 + screen1.rows - roiHeight;

                if (abs(newCenterX - oldCenterX) < 10 && abs(newCenterY - oldCenterY) < 10) {
                    isNewChange = false;
                    break;
                }
            }

            if (isNewChange) {
                for (int j = 0; j < 5; ++j) {
                    mouseClick(newCenterX, newCenterY);
                    Sleep(2);
                }
                it = newContours.erase(it);
            }
            else {
                ++it;
            }
        }

        if (newContours.empty()) {
            std::cout << "No new differences detected. Exiting loop." << std::endl;
            break;
        }

        contours = std::move(newContours);
        previousScreen = newScreen;
    }*/

    Sleep(100);
    key_.key1(27);
    key_.key1(86);

    Gdiplus::GdiplusShutdown(gdiplusToken);
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
            if (tokens[0].size() != 1)
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
#pragma once
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include <string>
class CoutButtonUser
{
public:
    sf::Vector2f findElementInImage(const std::string& templatePath);
private:
    cv::Mat captureScreen();
};



#pragma once

#include <fstream>

class LinuxLEDController
{
public:
    LinuxLEDController();
    ~LinuxLEDController();

    std::string GetRedPath();
    std::string GetBluePath();
    std::string GetGreenPath();

    void OpenRedPath(std::string red_path);
    void OpenGreenPath(std::string green_path);
    void OpenBluePath(std::string blue_path);

    void SetRGB(unsigned char red, unsigned char grn, unsigned char blu);
private:
    std::string     led_r_path;
    std::string     led_g_path;
    std::string     led_b_path;
    std::ofstream   led_r_brightness;
    std::ofstream   led_g_brightness;
    std::ofstream   led_b_brightness;
};

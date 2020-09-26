#include "LinuxLEDController.h"

LinuxLEDController::LinuxLEDController()
{

}

LinuxLEDController::~LinuxLEDController()
{

}

std::string LinuxLEDController::GetRedPath()
{
    return(led_r_path);
}

std::string LinuxLEDController::GetGreenPath()
{
    return(led_g_path);
}

std::string LinuxLEDController::GetBluePath()
{
    return(led_b_path);
}

void LinuxLEDController::OpenRedPath(std::string red_path)
{
    led_r_path = red_path;
    led_r_brightness.open(led_r_path + "brightness");
}

void LinuxLEDController::OpenGreenPath(std::string green_path)
{
    led_g_path = green_path;
    led_g_brightness.open(led_g_path + "brightness");
}

void LinuxLEDController::OpenBluePath(std::string blue_path)
{
    led_b_path = blue_path;
    led_b_brightness.open(led_b_path + "brightness");
}

void LinuxLEDController::SetRGB(unsigned char red, unsigned char grn, unsigned char blu)
{
    std::string brightness_str;

    /*-------------------------------------------------------------*\
    | My phone LED that I tested this on shuts down if you set zero |
    \*-------------------------------------------------------------*/
    if(red == 0) red = 1;
    if(grn == 0) grn = 1;
    if(blu == 0) blu = 1;

    brightness_str = std::to_string((unsigned int)red);

    led_r_brightness.write(brightness_str.c_str(), brightness_str.length());
    led_r_brightness.flush();

    brightness_str = std::to_string((unsigned int)grn);

    led_g_brightness.write(brightness_str.c_str(), brightness_str.length());
    led_g_brightness.flush();

    brightness_str = std::to_string((unsigned int)blu);

    led_b_brightness.write(brightness_str.c_str(), brightness_str.length());
    led_b_brightness.flush();
}

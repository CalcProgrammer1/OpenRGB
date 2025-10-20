/*---------------------------------------------------------*\
| LinuxLEDController_Linux.cpp                              |
|                                                           |
|   Driver for Linux sysfs LEDs                             |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "LinuxLEDController_Linux.h"

LinuxLEDController::LinuxLEDController(std::string dev_name)
{
    name = dev_name;
}

LinuxLEDController::~LinuxLEDController()
{

}

std::string LinuxLEDController::GetName()
{
    return(name);
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

std::string LinuxLEDController::GetRgbPath()
{
    return(led_rgb_path);
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

void LinuxLEDController::OpenRgbPath(std::string rgb_path)
{
    led_rgb_path = rgb_path;
    led_rgb_brightness.open(led_rgb_path + "brightness");
    led_rgb_color.open(led_rgb_path + "multi_intensity");
}

void LinuxLEDController::SetRGB(unsigned char red, unsigned char grn, unsigned char blu)
{
    std::string brightness_str;

    if(led_rgb_path.empty())
    {
        /*-------------------------------------------------*\
        | My phone LED that I tested this on shuts down if  |
        | you set zero                                      |
        \*-------------------------------------------------*/
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
    else
    {
        /*-------------------------------------------------*\
        | For the led_classdev_mc brightness just applies a |
        | coefficient to the multi_intensity. Set brightness|
        | to maximum and use the RGB values directly        |
        | instead.                                          |
        \*-------------------------------------------------*/
        brightness_str = std::to_string((unsigned int)255);
        led_rgb_brightness.write(brightness_str.c_str(), brightness_str.length());
        led_rgb_brightness.flush();

        brightness_str = std::to_string((unsigned int)red) + " " + std::to_string((unsigned int)grn) + " " + std::to_string((unsigned int)blu);
        led_rgb_color.write(brightness_str.c_str(), brightness_str.length());
        led_rgb_color.flush();
    }
}

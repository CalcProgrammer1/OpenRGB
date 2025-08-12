/*---------------------------------------------------------*\
| LinuxLEDController_Linux.h                                |
|                                                           |
|   Driver for Linux sysfs LEDs                             |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <fstream>

class LinuxLEDController
{
public:
    LinuxLEDController(std::string dev_name);
    ~LinuxLEDController();

    std::string GetName();

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
    std::string     name;
};

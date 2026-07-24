/*---------------------------------------------------------*\
| LinuxRGBLEDController_Linux.cpp                           |
|                                                           |
|   Driver for Linux RGB sysfs LEDs (multi_intensity)       |
|                                                           |
|   This driver supports the Valve-style sysfs LED interface|
|   found at /sys/class/leds/valve-leds* where each LED     |
|   directory contains a multi_intensity file for RGB       |
|   control and an effect file for hardware effect          |
|   selection.                                              |
|                                                           |
|   Effects are exposed as modes.  Only the first LED needs |
|   to have the effect set, as it applies to all LEDs on    |
|   the device.                                             |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      23 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "LinuxRGBLEDController_Linux.h"
#include <fstream>
#include <sstream>
#include <algorithm>

LinuxRGBLEDController::LinuxRGBLEDController(std::string dev_name)
{
    name        = dev_name;
    has_effects = false;
}

LinuxRGBLEDController::~LinuxRGBLEDController()
{

}

std::string LinuxRGBLEDController::GetName()
{
    return(name);
}

void LinuxRGBLEDController::AddLED(std::string led_path)
{
    /*-----------------------------------------------------*\
    | Ensure the path ends with a slash                     |
    \*-----------------------------------------------------*/
    std::string path = led_path;

    if(path.back() != '/')
    {
        path += '/';
    }

    led_paths.push_back(path);

    /*-----------------------------------------------------*\
    | Open the brightness and multi_intensity files         |
    \*-----------------------------------------------------*/
    std::ofstream brightness_file;
    brightness_file.open(path + "brightness");
    led_brightness.push_back(std::move(brightness_file));

    std::ofstream multi_intensity_file;
    multi_intensity_file.open(path + "multi_intensity");
    led_multi_intensity.push_back(std::move(multi_intensity_file));

    /*-----------------------------------------------------*\
    | Try to open the effect file to check if effects are   |
    | supported by this LED                                 |
    \*-----------------------------------------------------*/
    std::ifstream effect_test(path + "effect");

    if(effect_test.good())
    {
        effect_test.close();

        std::ofstream effect_file;
        effect_file.open(path + "effect");
        led_effect.push_back(std::move(effect_file));

        /*-------------------------------------------------*\
        | Read available effects from the first LED only    |
        \*-------------------------------------------------*/
        if(led_paths.size() == 1)
        {
            ReadAvailableEffects(path);
        }
    }
    else
    {
        effect_test.close();
    }
}

size_t LinuxRGBLEDController::GetLEDCount()
{
    return(led_paths.size());
}

std::string LinuxRGBLEDController::GetLEDPath(unsigned int led_idx)
{
    if(led_idx < led_paths.size())
    {
        return(led_paths[led_idx]);
    }

    return("");
}

std::vector<std::string> LinuxRGBLEDController::GetAvailableEffects()
{
    return(available_effects);
}

bool LinuxRGBLEDController::SupportsEffects()
{
    return(has_effects);
}

void LinuxRGBLEDController::ReadAvailableEffects(std::string first_led_path)
{
    std::ifstream effect_file(first_led_path + "effect");

    if(!effect_file.good())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | The effect file contains a list of effect names       |
    | separated by whitespace.  The currently active effect |
    | is enclosed in brackets, e.g. [none] solid rainbow    |
    \*-----------------------------------------------------*/
    std::string content;
    std::string line;

    while(std::getline(effect_file, line))
    {
        content += line + " ";
    }

    effect_file.close();

    std::istringstream stream(content);
    std::string token;

    while(stream >> token)
    {
        /*-------------------------------------------------*\
        | Remove brackets from the currently active effect  |
        | name                                              |
        \*-------------------------------------------------*/
        if(token.front() == '[' && token.back() == ']')
        {
            token = token.substr(1, token.size() - 2);
        }

        /*-------------------------------------------------*\
        | Add to list if not already present                |
        \*-------------------------------------------------*/
        if(std::find(available_effects.begin(), available_effects.end(), token) == available_effects.end())
        {
            available_effects.push_back(token);
        }
    }

    has_effects = (available_effects.size() > 0);
}

void LinuxRGBLEDController::SetLEDColor(unsigned int led_idx, unsigned char red, unsigned char grn, unsigned char blu)
{
    if(led_idx >= led_paths.size())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Set brightness to maximum and use multi_intensity for |
    | the actual RGB values.  This is the Valve sysfs LED   |
    | interface pattern.                                    |
    \*-----------------------------------------------------*/
    std::string brightness_str = std::to_string((unsigned int)255);
    led_brightness[led_idx].write(brightness_str.c_str(), brightness_str.length());
    led_brightness[led_idx].flush();

    std::string color_str = std::to_string((unsigned int)red)   + " "
                          + std::to_string((unsigned int)grn) + " "
                          + std::to_string((unsigned int)blu);
    led_multi_intensity[led_idx].write(color_str.c_str(), color_str.length());
    led_multi_intensity[led_idx].flush();
}

void LinuxRGBLEDController::SetLEDEffect(unsigned int led_idx, std::string effect)
{
    if(led_idx >= led_paths.size())
    {
        return;
    }

    if(led_effect.size() <= led_idx)
    {
        return;
    }

    led_effect[led_idx].write(effect.c_str(), effect.length());
    led_effect[led_idx].flush();
}

void LinuxRGBLEDController::SetEffect(std::string effect)
{
    /*-----------------------------------------------------*\
    | Setting the effect on one LED applies it to all LEDs  |
    | on the device, so we only need to set it on the first |
    | LED                                                   |
    \*-----------------------------------------------------*/
    if(led_effect.size() > 0)
    {
        led_effect[0].write(effect.c_str(), effect.length());
        led_effect[0].flush();
    }
}
/*---------------------------------------------------------*\
| ValveSteamMachineController_Linux.cpp                     |
|                                                           |
|   Driver for Valve Steam Machine LEDs                     |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      23 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "ValveSteamMachineController_Linux.h"
#include <fstream>
#include <sstream>
#include <algorithm>

ValveSteamMachineController::ValveSteamMachineController(std::string dev_name)
{
    name        = dev_name;
}

ValveSteamMachineController::~ValveSteamMachineController()
{

}

std::string ValveSteamMachineController::GetName()
{
    return(name);
}

void ValveSteamMachineController::AddLED(std::string led_path)
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
    | Open the brightness_scale and delay files on the      |
    | first LED only.  Setting these on any one LED applies |
    | to the entire strip, so we only need to open it once. |
    \*-----------------------------------------------------*/
    if(led_paths.size() == 1)
    {
        led_brightness_scale.open(path + "brightness_scale");
        led_delay.open(path + "delay");
    }

    /*-----------------------------------------------------*\
    | Check if the effect_index file exists to determine    |
    | if hardware effects are supported by this LED.  The   |
    | effect_index file contains a list of available effect |
    | names, while the effect file is used to set the       |
    | active effect.                                        |
    \*-----------------------------------------------------*/
    std::ifstream effect_index_test(path + "effect_index");

    if(effect_index_test.good())
    {
        effect_index_test.close();

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
        effect_index_test.close();
    }
}

size_t ValveSteamMachineController::GetLEDCount()
{
    return(led_paths.size());
}

std::string ValveSteamMachineController::GetLEDPath(unsigned int led_idx)
{
    if(led_idx < led_paths.size())
    {
        return(led_paths[led_idx]);
    }

    return("");
}

std::vector<std::string> ValveSteamMachineController::GetAvailableEffects()
{
    return(available_effects);
}

void ValveSteamMachineController::ReadAvailableEffects(std::string first_led_path)
{
    std::ifstream effect_index_file(first_led_path + "effect_index");

    if(!effect_index_file.good())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | The effect_index file contains a list of available    |
    | effect names separated by whitespace.  The effect     |
    | file itself gives the currently active effect when    |
    | read, and is used to set the active effect when       |
    | written.                                              |
    \*-----------------------------------------------------*/
    std::string content;
    std::string line;

    while(std::getline(effect_index_file, line))
    {
        content += line + " ";
    }

    effect_index_file.close();

    std::istringstream stream(content);
    std::string token;

    while(stream >> token)
    {
        /*-------------------------------------------------*\
        | Add to list if not already present                |
        \*-------------------------------------------------*/
        if(std::find(available_effects.begin(), available_effects.end(), token) == available_effects.end())
        {
            available_effects.push_back(token);
        }
    }
}

void ValveSteamMachineController::SetLEDColor(unsigned int led_idx, unsigned char red, unsigned char grn, unsigned char blu)
{
    if(led_idx >= led_paths.size())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Use multi_intensity to set RGB values.  This is the   |
    | leds-valve sysfs LED interface pattern.               |
    \*-----------------------------------------------------*/
    std::string color_str = std::to_string((unsigned int)red) + " "
                          + std::to_string((unsigned int)grn) + " "
                          + std::to_string((unsigned int)blu);

    led_multi_intensity[led_idx].write(color_str.c_str(), color_str.length());
    led_multi_intensity[led_idx].flush();
}

void ValveSteamMachineController::SetLEDEffect(unsigned int led_idx, std::string effect)
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

void ValveSteamMachineController::SetEffect(std::string effect)
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

void ValveSteamMachineController::SetBrightness(unsigned int brightness)
{
    if(led_brightness_scale.is_open())
    {
        std::string brightness_str = std::to_string(brightness);
        led_brightness_scale.write(brightness_str.c_str(), brightness_str.length());
        led_brightness_scale.flush();
    }
}

void ValveSteamMachineController::SetDelay(unsigned int delay)
{
    if(led_delay.is_open())
    {
        std::string delay_str = std::to_string(delay);
        led_delay.write(delay_str.c_str(), delay_str.length());
        led_delay.flush();
    }
}
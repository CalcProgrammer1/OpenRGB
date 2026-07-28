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
    | Open the multi_intensity files                        |
    \*-----------------------------------------------------*/
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
        led_enabled.open(path + "enabled");
        led_breath_offset.open(path + "breath_offset");
        led_breath_level.open(path + "breath_level");
        led_patrol_num.open(path + "patrol_num");
        led_color_shift.open(path + "color_shift");
        led_multi_intensity_startup.open(path + "multi_intensity_startup");
        led_brightness_startup.open(path + "brightness_startup");
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

std::string ValveSteamMachineController::GetLocation()
{
    return(led_paths[0]);
}

std::vector<std::string> ValveSteamMachineController::GetAvailableEffects()
{
    return(available_effects);
}

unsigned int ValveSteamMachineController::GetBrightness()
{
    if(led_brightness_scale.is_open())
    {
        std::ifstream brightness_file(led_paths[0] + "brightness_scale");

        if(brightness_file.good())
        {
            unsigned int brightness = 0;
            brightness_file >> std::hex >> brightness;
            brightness_file.close();
            return(brightness);
        }
    }

    return(0);
}

unsigned int ValveSteamMachineController::GetDelay()
{
    if(led_delay.is_open())
    {
        std::ifstream delay_file(led_paths[0] + "delay");

        if(delay_file.good())
        {
            unsigned int delay = 0;
            delay_file >> std::hex >> delay;
            delay_file.close();
            return(delay);
        }
    }

    return(0);
}

std::string ValveSteamMachineController::GetEffect()
{
    /*-----------------------------------------------------*\
    | Reading the effect on one LED reflects the active     |
    | effect for all LEDs on the device, so we only need to |
    | read it from the first LED.                           |
    \*-----------------------------------------------------*/
    if(led_effect.size() > 0)
    {
        std::ifstream effect_file(led_paths[0] + "effect");

        if(effect_file.good())
        {
            std::string effect;
            std::getline(effect_file, effect);
            effect_file.close();
            return(effect);
        }
    }

    return("");
}

bool ValveSteamMachineController::GetEnabled()
{
    if(led_enabled.is_open())
    {
        std::ifstream enabled_file(led_paths[0] + "enabled");

        if(enabled_file.good())
        {
            unsigned int enabled = 0;
            enabled_file >> std::hex >> enabled;
            enabled_file.close();
            return(enabled != 0);
        }
    }

    return(false);
}

unsigned int ValveSteamMachineController::GetBreathOffset()
{
    if(led_breath_offset.is_open())
    {
        std::ifstream breath_offset_file(led_paths[0] + "breath_offset");

        if(breath_offset_file.good())
        {
            unsigned int breath_offset = 0;
            breath_offset_file >> std::hex >> breath_offset;
            breath_offset_file.close();
            return(breath_offset);
        }
    }

    return(0);
}

unsigned int ValveSteamMachineController::GetBreathLevel()
{
    if(led_breath_level.is_open())
    {
        std::ifstream breath_level_file(led_paths[0] + "breath_level");

        if(breath_level_file.good())
        {
            unsigned int breath_level = 0;
            breath_level_file >> std::hex >> breath_level;
            breath_level_file.close();
            return(breath_level);
        }
    }

    return(0);
}

unsigned int ValveSteamMachineController::GetPatrolNum()
{
    if(led_patrol_num.is_open())
    {
        std::ifstream patrol_num_file(led_paths[0] + "patrol_num");

        if(patrol_num_file.good())
        {
            unsigned int patrol_num = 0;
            patrol_num_file >> std::hex >> patrol_num;
            patrol_num_file.close();
            return(patrol_num);
        }
    }

    return(0);
}

unsigned int ValveSteamMachineController::GetColorShift()
{
    if(led_color_shift.is_open())
    {
        std::ifstream color_shift_file(led_paths[0] + "color_shift");

        if(color_shift_file.good())
        {
            unsigned int color_shift = 0;
            color_shift_file >> std::hex >> color_shift;
            color_shift_file.close();
            return(color_shift);
        }
    }

    return(0);
}

RGBColor ValveSteamMachineController::GetStartupColor()
{
    if(led_multi_intensity_startup.is_open())
    {
        std::ifstream startup_color_file(led_paths[0] + "multi_intensity_startup");

        if(startup_color_file.good())
        {
            unsigned int red = 0;
            unsigned int grn = 0;
            unsigned int blu = 0;
            startup_color_file >> std::hex >> red >> grn >> blu;
            startup_color_file.close();
            return(ToRGBColor(red, grn, blu));
        }
    }

    return(0);
}

unsigned int ValveSteamMachineController::GetBrightnessStartup()
{
    if(led_brightness_startup.is_open())
    {
        std::ifstream brightness_startup_file(led_paths[0] + "brightness_startup");

        if(brightness_startup_file.good())
        {
            unsigned int brightness_startup = 0;
            brightness_startup_file >> std::hex >> brightness_startup;
            brightness_startup_file.close();
            return(brightness_startup);
        }
    }

    return(0);
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
    std::string token;

    while(effect_index_file >> token)
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

void ValveSteamMachineController::SetLEDColor(unsigned int led_idx, RGBColor color)
{
    if(led_idx >= led_paths.size())
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Use multi_intensity to set RGB values.  This is the   |
    | leds-valve sysfs LED interface pattern.               |
    \*-----------------------------------------------------*/
    std::string color_str = std::to_string(RGBGetRValue(color)) + " "
                          + std::to_string(RGBGetGValue(color)) + " "
                          + std::to_string(RGBGetBValue(color));

    led_multi_intensity[led_idx].write(color_str.c_str(), color_str.length());
    led_multi_intensity[led_idx].flush();
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

void ValveSteamMachineController::SetEnabled(bool enabled)
{
    if(led_enabled.is_open())
    {
        std::string enabled_str = std::to_string(enabled ? 1 : 0);
        led_enabled.write(enabled_str.c_str(), enabled_str.length());
        led_enabled.flush();
    }
}

void ValveSteamMachineController::SetBreathOffset(unsigned int breath_offset)
{
    if(led_breath_offset.is_open())
    {
        std::string breath_offset_str = std::to_string(breath_offset);
        led_breath_offset.write(breath_offset_str.c_str(), breath_offset_str.length());
        led_breath_offset.flush();
    }
}

void ValveSteamMachineController::SetBreathLevel(unsigned int breath_level)
{
    if(led_breath_level.is_open())
    {
        std::string breath_level_str = std::to_string(breath_level);
        led_breath_level.write(breath_level_str.c_str(), breath_level_str.length());
        led_breath_level.flush();
    }
}

void ValveSteamMachineController::SetPatrolNum(unsigned int patrol_num)
{
    if(led_patrol_num.is_open())
    {
        std::string patrol_num_str = std::to_string(patrol_num);
        led_patrol_num.write(patrol_num_str.c_str(), patrol_num_str.length());
        led_patrol_num.flush();
    }
}

void ValveSteamMachineController::SetColorShift(unsigned int color_shift)
{
    if(led_color_shift.is_open())
    {
        std::string color_shift_str = std::to_string(color_shift);
        led_color_shift.write(color_shift_str.c_str(), color_shift_str.length());
        led_color_shift.flush();
    }
}

void ValveSteamMachineController::SetStartupColor(RGBColor color)
{
    if(led_multi_intensity_startup.is_open())
    {
        /*-----------------------------------------------------*\
        | Use multi_intensity_startup to set RGB values.  This  |
        | is the leds-valve sysfs LED interface pattern for     |
        | the startup color.                                    |
        \*-----------------------------------------------------*/
        std::string color_str = std::to_string(RGBGetRValue(color)) + " "
                              + std::to_string(RGBGetGValue(color)) + " "
                              + std::to_string(RGBGetBValue(color));

        led_multi_intensity_startup.write(color_str.c_str(), color_str.length());
        led_multi_intensity_startup.flush();
    }
}

void ValveSteamMachineController::SetBrightnessStartup(unsigned int brightness_startup)
{
    if(led_brightness_startup.is_open())
    {
        std::string brightness_startup_str = std::to_string(brightness_startup);
        led_brightness_startup.write(brightness_startup_str.c_str(), brightness_startup_str.length());
        led_brightness_startup.flush();
    }
}

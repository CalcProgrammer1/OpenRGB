/*---------------------------------------------------------*\
| ValveSteamMachineController_Linux.h                       |
|                                                           |
|   Driver for Valve Steam Machine LEDs                     |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      23 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <fstream>
#include <string>
#include <vector>

class ValveSteamMachineController
{
public:
    ValveSteamMachineController(std::string dev_name);
    ~ValveSteamMachineController();

    std::string                 GetName();

    void                        AddLED(std::string led_path);
    size_t                      GetLEDCount();

    std::string                 GetLEDPath(unsigned int led_idx);

    std::vector<std::string>    GetAvailableEffects();
    unsigned int                GetBrightness();
    unsigned int                GetDelay();
    std::string                 GetEffect();

    bool                        SupportsEffects();

    void                        SetLEDColor(unsigned int led_idx, unsigned char red, unsigned char grn, unsigned char blu);
    void                        SetEffect(std::string effect);
    void                        SetBrightness(unsigned int brightness);
    void                        SetDelay(unsigned int delay);

private:
    std::string                 name;

    std::vector<std::string>    led_paths;
    std::vector<std::ofstream>  led_brightness;
    std::vector<std::ofstream>  led_multi_intensity;
    std::vector<std::ofstream>  led_effect;
    std::ofstream               led_brightness_scale;
    std::ofstream               led_delay;
    std::vector<std::string>    available_effects;

    void                        ReadAvailableEffects(std::string first_led_path);
};

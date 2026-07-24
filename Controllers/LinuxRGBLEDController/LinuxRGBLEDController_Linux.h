/*---------------------------------------------------------*\
| LinuxRGBLEDController_Linux.h                             |
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

#pragma once

#include <fstream>
#include <string>
#include <vector>

class LinuxRGBLEDController
{
public:
    LinuxRGBLEDController(std::string dev_name);
    ~LinuxRGBLEDController();

    std::string                 GetName();

    void                        AddLED(std::string led_path);
    size_t                      GetLEDCount();

    std::string                 GetLEDPath(unsigned int led_idx);

    std::vector<std::string>    GetAvailableEffects();
    bool                        SupportsEffects();

    void                        SetLEDColor(unsigned int led_idx, unsigned char red, unsigned char grn, unsigned char blu);
    void                        SetLEDEffect(unsigned int led_idx, std::string effect);
    void                        SetEffect(std::string effect);

private:
    std::string                 name;
    std::vector<std::string>    led_paths;
    std::vector<std::ofstream>  led_brightness;
    std::vector<std::ofstream>  led_multi_intensity;
    std::vector<std::ofstream>  led_effect;
    std::vector<std::string>    available_effects;
    bool                        has_effects;

    void                        ReadAvailableEffects(std::string first_led_path);
};
/*---------------------------------------------------------*\
| ValveSteamMachineController_Windows.h                     |
|                                                           |
|   Driver for Valve Steam Machine LEDs                     |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      28 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "PawnIOLib.h"
#include "RGBControllerInterface.h"

class ValveSteamMachineController
{
public:
    ValveSteamMachineController(HANDLE dev_handle, std::string dev_name);
    ~ValveSteamMachineController();

    std::string                 GetName();
    size_t                      GetLEDCount();
    std::string                 GetLocation();

    std::vector<std::string>    GetAvailableEffects();
    unsigned int                GetBrightness();
    unsigned int                GetDelay();
    std::string                 GetEffect();
    bool                        GetEnabled();
    unsigned int                GetBreathOffset();
    unsigned int                GetBreathLevel();
    unsigned int                GetPatrolNum();
    unsigned int                GetColorShift();
    bool                        GetPersistence();
    RGBColor                    GetStartupColor();
    unsigned int                GetBrightnessStartup();

    bool                        SupportsEffects();

    void                        SetLEDColor(unsigned int led_idx, RGBColor color);
    void                        SetEffect(std::string effect);
    void                        SetBrightness(unsigned int brightness);
    void                        SetDelay(unsigned int delay);
    void                        SetEnabled(bool enabled);
    void                        SetBreathOffset(unsigned int breath_offset);
    void                        SetBreathLevel(unsigned int breath_level);
    void                        SetPatrolNum(unsigned int patrol_num);
    void                        SetColorShift(unsigned int color_shift);
    void                        SetStartupColor(RGBColor color);
    void                        SetBrightnessStartup(unsigned int brightness_startup);

private:
    std::string                 name;
    HANDLE                      pawnio_handle;
};
/*---------------------------------------------------------*\
| CMKeyboardV2Controller.h                                  |
|                                                           |
|   Driver for Cooler Master V2 keyboards                   |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "CMKeyboardAbstractController.h"

struct stCMKeyboardV2_mode
{
    const char      *name;
    unsigned int    value;
    unsigned int    speed_min;
    unsigned int    speed_max;
    unsigned int    speed;
    unsigned int    brightness;
    unsigned int    direction;
    unsigned int    nColors;
    unsigned int    color_mode;
    unsigned int    flags;
};

class CMKeyboardV2Controller : public CMKeyboardAbstractController
{
public:
    CMKeyboardV2Controller(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name);
    ~CMKeyboardV2Controller();

    /*---------------------------------------------------------*\
    | Protocol specific funtions to be implmented               |
    \*---------------------------------------------------------*/
    void            SetLeds(std::vector<led> leds, std::vector<RGBColor> colors);
    void            SetSingleLED(uint8_t in_led, RGBColor in_color);
    void            Initialize();
    void            Shutdown();
    void            SetLEDControl(bool bManual);
    void            SendApplyPacket(uint8_t mode);
    void            MagicStartupPacket();
    void            MagicCommand(uint8_t profileId);
    void            SetCustomMode();
    void            SetMode(mode selectedMode);
    void            SetEffect(uint8_t effectId, uint8_t p1, uint8_t p2, uint8_t p3, RGBColor color1, RGBColor color2);
    void            InitializeModes(std::vector<mode> &modes);
    KEYBOARD_LAYOUT GetKeyboardLayout();

private:
    void            _SetEffectMode(uint8_t effectId);
    void            _UpdateSpeed(mode selectedMode, uint8_t &cSpeed1, uint8_t &cSpeed2);
    std::string     _GetFirmwareVersion();

    bool            m_bMoreFFs;
};

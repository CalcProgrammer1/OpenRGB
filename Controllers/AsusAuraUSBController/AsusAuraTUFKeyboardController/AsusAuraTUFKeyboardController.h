/*---------------------------------------------------------*\
| AsusAuraTUFKeyboardController.h                           |
|                                                           |
|   Driver for ASUS Aura TUF keyboard                       |
|                                                           |
|   Mola19                                      03 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"
#include "AsusAuraTUFKeyboardLayouts.h"

enum
{
    AURA_KEYBOARD_MODE_STATIC       =  0,
    AURA_KEYBOARD_MODE_BREATHING    =  1,
    AURA_KEYBOARD_MODE_COLOR_CYCLE  =  2,
    AURA_KEYBOARD_MODE_REACTIVE     =  3,
    AURA_KEYBOARD_MODE_WAVE         =  4,
    AURA_KEYBOARD_MODE_RIPPLE       =  5,
    AURA_KEYBOARD_MODE_STARRY_NIGHT =  6,
    AURA_KEYBOARD_MODE_QUICKSAND    =  7,
    AURA_KEYBOARD_MODE_CURRENT      =  8,
    AURA_KEYBOARD_MODE_RAIN_DROP    =  9,
    AURA_KEYBOARD_MODE_DIRECT       = 15,
};

enum
{
    AURA_ROG_AZOTH_USB_PID                              = 0x1A83,
    AURA_ROG_AZOTH_2_4_PID                              = 0x1A85,
    AURA_ROG_CLAYMORE_PID                               = 0x184D,
    AURA_ROG_FALCHION_WIRED_PID                         = 0x193C,
    AURA_ROG_FALCHION_WIRELESS_PID                      = 0x193E,
    AURA_ROG_STRIX_FLARE_PID                            = 0x1875,
    AURA_ROG_STRIX_FLARE_PNK_LTD_PID                    = 0x18CF,
    AURA_ROG_STRIX_FLARE_COD_BO4_PID                    = 0x18AF,
    AURA_ROG_STRIX_FLARE_II_PID                         = 0x19FE,
    AURA_ROG_STRIX_FLARE_II_ANIMATE_PID                 = 0x19FC,
    AURA_ROG_STRIX_SCOPE_PID                            = 0x18F8,
    AURA_ROG_STRIX_SCOPE_RX_PID                         = 0x1951,
    AURA_ROG_STRIX_SCOPE_RX_EVA_02_PID                  = 0x1B12,
    AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_USB_PID     = 0x19F6,
    AURA_ROG_STRIX_SCOPE_NX_WIRELESS_DELUXE_2_4_PID     = 0x19F8,
    AURA_ROG_STRIX_SCOPE_II_PID                         = 0x1AB3,
    AURA_ROG_STRIX_SCOPE_II_RX_PID                      = 0x1AB5,
    AURA_ROG_STRIX_SCOPE_II_96_WIRELESS_USB_PID         = 0x1AAE,
    AURA_TUF_K1_GAMING_PID                              = 0x1945,
    AURA_TUF_K3_GAMING_PID                              = 0x194B,
    AURA_TUF_K5_GAMING_PID                              = 0x1899,
    AURA_TUF_K7_GAMING_PID                              = 0x18AA,
};

struct led_color
{
    unsigned int    value;
    RGBColor        color;
};

class AuraTUFKeyboardController
{
public:
    AuraTUFKeyboardController(hid_device* dev_handle, const char* path, uint16_t pid, unsigned short version, std::string dev_name);
    ~AuraTUFKeyboardController();

    std::string GetDeviceLocation();
    std::string GetName();
    std::string GetSerialString();
    std::string GetVersion();

    int         GetLayout();
    int         GetNumpadLocation();
    void        SaveMode();
    void        AllowRemoteControl(unsigned char type);

    void        UpdateSingleLed
                    (
                    int             led,
                    unsigned char   red,
                    unsigned char   green,
                    unsigned char   blue
                    );

    void        UpdateLeds
                    (
                    std::vector<led_color>    colors
                    );

    void        UpdateK1Wave
                    (
                    std::vector<RGBColor>   colors,
                    unsigned char           direction,
                    unsigned char           speed,
                    unsigned char           brightness
                    );

    void        UpdateScopeIIRainbowRipple
                    (
                    unsigned char           mode,
                    std::vector<RGBColor>   colors,
                    unsigned char           direction,
                    unsigned char           color_mode,
                    unsigned char           speed,
                    unsigned char           brightness
                    );

    void        UpdateScopeIIQuicksand
                    (
                    std::vector<RGBColor>   colors,
                    unsigned char           direction,
                    unsigned char           color_mode,
                    unsigned char           speed,
                    unsigned char           brightness
                    );

    void        UpdateDevice
                    (
                    unsigned char           mode,
                    std::vector<RGBColor>   colors,
                    unsigned char           direction,
                    unsigned char           color_mode,
                    unsigned char           speed,
                    unsigned char           brightness
                    );

    void        UpdateQuicksandColors(std::vector<RGBColor> colors);
    void        UpdateMode(unsigned char mode);
    void        AwaitResponse(int ms);
    void        ClearResponses();

    uint16_t    device_pid;
    bool        is_per_led_keyboard;

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
    unsigned short  rev_version;
};


/*-----------------------------------------*\
|  AsusAuraTUFKeyboardController.h          |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Mola19 03/03/2021                        |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    AURA_KEYBOARD_ZONE_LOGO         = 0,
    AURA_KEYBOARD_ZONE_SCROLL       = 1,
    AURA_KEYBOARD_ZONE_UNDERGLOW    = 2,
    AURA_KEYBOARD_ZONE_ALL          = 3,
};

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

class AuraTUFKeyboardController
{
public:
    AuraTUFKeyboardController(hid_device* dev_handle, const char* path);
    ~AuraTUFKeyboardController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void UpdateSingleLed
        (
        int             led,
        unsigned char   red,
        unsigned char   green,
        unsigned char   blue
        );

	void UpdateLeds
        (
        std::vector<RGBColor>    colors
        );

    void UpdateDevice
        (
        unsigned char           mode,
        std::vector<RGBColor>   colors,
        unsigned char           dir,
        unsigned char           color_mode,
        unsigned char           speed
        );

private:
    hid_device*                 dev;
    std::string                 location;
};


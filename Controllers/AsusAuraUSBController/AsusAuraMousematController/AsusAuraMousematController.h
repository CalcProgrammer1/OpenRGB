/*-----------------------------------------*\
|  AsusAuraMousematController.h             |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/10/2022   |
\*-----------------------------------------*/

#include "RGBController.h"
#include <hidapi/hidapi.h>

#pragma once


enum
{
    AURA_MOUSEMAT_MODE_STATIC           = 0,
    AURA_MOUSEMAT_MODE_BREATHING        = 1,
    AURA_MOUSEMAT_MODE_COLOR_CYCLE      = 2,
    AURA_MOUSEMAT_MODE_WAVE             = 3,
    AURA_MOUSEMAT_MODE_WAVE_PLANE       = 4,
    AURA_MOUSEMAT_MODE_COMET            = 5,
    AURA_MOUSEMAT_MODE_GLOWING_YOYO     = 6,
    AURA_MOUSEMAT_MODE_CROSS            = 7,
    AURA_MOUSEMAT_MODE_STARRY_NIGHT     = 8,
    AURA_MOUSEMAT_MODE_DIRECT           = 0xFF,
};

class AuraMousematController
{
public:
    AuraMousematController(hid_device* dev_handle, const char* path);
    virtual ~AuraMousematController();

    std::string GetDeviceLocation();
    std::string GetSerialString();
    std::string GetVersion();

    void UpdateLeds
        (
        std::vector<RGBColor>    colors
        );

    void UpdateDevice
        (
        unsigned char           mode,
        std::vector<RGBColor>   colors,
        unsigned char           speed,
        unsigned char           brightness,
        unsigned char           pattern
        );

    void SaveMode();

private:
    hid_device*                 dev;
    std::string                 location;
};

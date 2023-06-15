/*-----------------------------------------*\
|  AsusROGAllyController.h                  |
|                                           |
|  Definitions and types for ASUS ROG Ally  |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/12/2023   |
\*-----------------------------------------*/

#include "RGBController.h"
#include <hidapi/hidapi.h>

#pragma once

enum
{
    ROG_ALLY_MODE_STATIC           = 0,
    ROG_ALLY_MODE_BREATHING        = 1,
    ROG_ALLY_MODE_COLOR_CYCLE      = 2,
    ROG_ALLY_MODE_WAVE             = 3,
    ROG_ALLY_MODE_STROBING         = 10,
    ROG_ALLY_MODE_DIRECT           = 0xFF,
};

class ROGAllyController
{
public:
    ROGAllyController(hid_device* dev_handle, const char* path);
    virtual ~ROGAllyController();

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

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
    AURA_MOUSEMAT_MODE_DIRECT           = 0,
    AURA_MOUSEMAT_MODE_BREATHING        = 1,
    AURA_MOUSEMAT_MODE_CHASING          = 3,
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
        unsigned char   mode,
        unsigned char   red,
        unsigned char   grn,
        unsigned char   blu,
        unsigned char   speed,
        unsigned char   brightness
        );

    void SaveMode();

private:
    hid_device*                 dev;
    std::string                 location;
};

/*-----------------------------------------*\
|  AsusAuraMouseController.h                |
|                                           |
|  Definitions and types for ASUS Aura      |
|  USB RGB lighting controller              |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/23/2020  |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    AURA_MOUSE_ZONE_LOGO        = 0,
    AURA_MOUSE_ZONE_SCROLL      = 1,
    AURA_MOUSE_ZONE_UNDERGLOW   = 2,
    AURA_MOUSE_ZONE_ALL         = 3,
};

enum
{
    AURA_MOUSE_MODE_STATIC      = 0,
    AURA_MOUSE_MODE_BREATHING   = 1,
    AURA_MOUSE_MODE_COLOR_CYCLE = 2,
    AURA_MOUSE_MODE_REACTIVE    = 3,
};

class AuraMouseController
{
public:
    AuraMouseController(hid_device* dev_handle, const char* path);
    virtual ~AuraMouseController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendUpdate
        (
        unsigned char   zone,
        unsigned char   mode,
        unsigned char   red,
        unsigned char   grn,
        unsigned char   blu
        );

private:
    hid_device*                 dev;
    std::string                 location;
};

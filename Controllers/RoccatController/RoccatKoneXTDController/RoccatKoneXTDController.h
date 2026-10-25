/*---------------------------------------------------------*\
| RoccatKoneXTDController.h                                 |
|                                                           |
|   Driver for Roccat Kone XTD                              |
|                                                           |
|   Julian                                      14 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define ROCCAT_KONE_XTD_NUMBER_OF_LEDS      4
#define ROCCAT_KONE_XTD_SPEED_MIN           1
#define ROCCAT_KONE_XTD_SPEED_MAX           3

/*---------------------------------------------------------*\
| Profile settings (report 0x06) light_effect_type          |
\*---------------------------------------------------------*/
enum
{
    ROCCAT_KONE_XTD_EFFECT_OFF          = 0x00,
    ROCCAT_KONE_XTD_EFFECT_FULLY_LIT    = 0x01,
    ROCCAT_KONE_XTD_EFFECT_BLINKING     = 0x02,
    ROCCAT_KONE_XTD_EFFECT_BREATHING    = 0x03,
    ROCCAT_KONE_XTD_EFFECT_HEARTBEAT    = 0x04,
};

/*---------------------------------------------------------*\
| Profile settings (report 0x06) color_flow_effect          |
\*---------------------------------------------------------*/
enum
{
    ROCCAT_KONE_XTD_FLOW_OFF            = 0x00,
    ROCCAT_KONE_XTD_FLOW_SIMULTANEOUS   = 0x01,
    ROCCAT_KONE_XTD_FLOW_UP             = 0x02,
    ROCCAT_KONE_XTD_FLOW_DOWN           = 0x03,
    ROCCAT_KONE_XTD_FLOW_LEFT           = 0x04,
    ROCCAT_KONE_XTD_FLOW_RIGHT          = 0x05,
};

/*---------------------------------------------------------*\
| Profile settings (report 0x06) light_effect_mode          |
\*---------------------------------------------------------*/
enum
{
    ROCCAT_KONE_XTD_COLOR_SELECTED      = 0x01,
    ROCCAT_KONE_XTD_COLOR_RANDOM_SINGLE = 0x02,
    ROCCAT_KONE_XTD_COLOR_RANDOM_MULTI  = 0x03,
};

class RoccatKoneXTDController
{
public:
    RoccatKoneXTDController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~RoccatKoneXTDController();

    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerial();
    std::string     GetVersion();

    /*-----------------------------------------------------*\
    | Live colors (report 0x10), not saved. colors: one per |
    | LED (ROCCAT_KONE_XTD_NUMBER_OF_LEDS)                  |
    \*-----------------------------------------------------*/
    void            SendDirectColors(const std::vector<RGBColor>& colors);

    /*-----------------------------------------------------*\
    | Hand the LEDs back to the active profile's lighting   |
    \*-----------------------------------------------------*/
    void            ReleaseDirect();

    /*-----------------------------------------------------*\
    | Write the lighting of the active profile (report      |
    | 0x06). This is stored on the mouse.                   |
    \*-----------------------------------------------------*/
    void            SetProfileLighting(bool                         lights_on,
                                       unsigned char                effect_type,
                                       unsigned char                color_flow,
                                       unsigned char                color_mode,
                                       unsigned char                speed,
                                       const std::vector<RGBColor>& colors);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
    std::string     version;

    unsigned char   GetActiveProfile();
    bool            WaitReady();
};

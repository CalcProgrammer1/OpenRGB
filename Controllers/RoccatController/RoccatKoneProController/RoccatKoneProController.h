/*---------------------------------------------------------*\
| RoccatKoneProController.h                                 |
|                                                           |
|   Driver for Roccat Kone Pro                              |
|                                                           |
|   Garrett Denham (GardenOfWyers)              12 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

#define ROCCAT_KONE_PRO_CONTROL_MODE_PACKET_LENGTH    6
#define ROCCAT_KONE_PRO_DIRECT_MODE_PACKET_LENGTH     11
#define ROCCAT_KONE_PRO_FLASH_PACKET_LENGTH           69
#define ROCCAT_KONE_PRO_FLASH_REPORT_ID               0x06
#define ROCCAT_KONE_PRO_DIRECT_MODE_REPORT_ID         0x0D
#define ROCCAT_KONE_PRO_DIRECT_MODE_BYTE              0x0B
#define ROCCAT_KONE_PRO_LED_COUNT                     2

enum
{
    ROCCAT_KONE_PRO_DIRECT_MODE_VALUE     = 0x00,
    ROCCAT_KONE_PRO_STATIC_MODE_VALUE     = 0x01,
    ROCCAT_KONE_PRO_WAVE_MODE_VALUE       = 0x0A,
    ROCCAT_KONE_PRO_HEARTBEAT_MODE_VALUE  = 0x04,
    ROCCAT_KONE_PRO_BREATHING_MODE_VALUE  = 0x03,
    ROCCAT_KONE_PRO_BLINKING_MODE_VALUE   = 0x02
};

enum
{
    ROCCAT_KONE_PRO_SPEED_MIN             = 0x01,
    ROCCAT_KONE_PRO_SPEED_MAX             = 0x0B,
    ROCCAT_KONE_PRO_SPEED_MID             = (ROCCAT_KONE_PRO_SPEED_MAX - ROCCAT_KONE_PRO_SPEED_MIN) / 2,
    ROCCAT_KONE_PRO_BRIGHTNESS_MIN        = 0x00,
    ROCCAT_KONE_PRO_BRIGHTNESS_MAX        = 0xFF
};

class RoccatKoneProController
{
public:
    RoccatKoneProController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~RoccatKoneProController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            SetupDirectMode();
    void            SendDirect(std::vector<RGBColor> colors);
    void            SetMode(std::vector<RGBColor> colors,
                            unsigned char mode_value,
                            unsigned char speed,
                            unsigned char brightness,
                            unsigned int color_mode,
                            unsigned int mode_flags
                            );
private:
    hid_device*     dev;
    std::string     location;
    std::string     name;

    unsigned int    CalculateCRC(unsigned char* bytes);
    void            SwitchControl(bool direct);
};

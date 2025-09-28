/*---------------------------------------------------------*\
| RoccatBurstController.h                                   |
|                                                           |
|   Driver for Roccat Burst                                 |
|                                                           |
|   Morgan Guimard (morg)                       01 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <hidapi.h>

#define ROCCAT_BURST_CONTROL_MODE_PACKET_LENGTH    6
#define ROCCAT_BURST_DIRECT_MODE_PACKET_LENGTH     11
#define ROCCAT_BURST_FLASH_PACKET_LENGTH           63
#define ROCCAT_BURST_FLASH_REPORT_ID               0x06
#define ROCCAT_BURST_DIRECT_MODE_REPORT_ID         0x0D
#define ROCCAT_BURST_DIRECT_MODE_BYTE              0x0B
#define ROCCAT_BURST_CORE_NUMBER_OF_LEDS           1
#define ROCCAT_BURST_PRO_NUMBER_OF_LEDS            2

enum
{
    ROCCAT_BURST_DIRECT_MODE_VALUE     = 0x00,
    ROCCAT_BURST_STATIC_MODE_VALUE     = 0x01,
    ROCCAT_BURST_WAVE_MODE_VALUE       = 0x0A,
    ROCCAT_BURST_HEARTBEAT_MODE_VALUE  = 0x04,
    ROCCAT_BURST_BREATHING_MODE_VALUE  = 0x03,
    ROCCAT_BURST_BLINKING_MODE_VALUE   = 0x02
};

enum
{
    ROCCAT_BURST_SPEED_MIN             = 0x01,
    ROCCAT_BURST_SPEED_MAX             = 0x0B,
    ROCCAT_BURST_BRIGHTNESS_MIN        = 0x00,
    ROCCAT_BURST_BRIGHTNESS_MAX        = 0xFF
};

class RoccatBurstController
{
public:
    RoccatBurstController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~RoccatBurstController();

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

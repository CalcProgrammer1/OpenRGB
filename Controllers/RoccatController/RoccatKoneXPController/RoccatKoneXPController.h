/*---------------------------------------------------------*\
| RoccatKoneXPController.h                                  |
|                                                           |
|   Driver for Roccat Kone XP                               |
|                                                           |
|   Mola19                                      12 Jun 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

enum
{
    ROCCAT_KONE_XP_MODE_DIRECT     = 0x0B,
    ROCCAT_KONE_XP_MODE_OFF        = 0x00,
    ROCCAT_KONE_XP_MODE_STATIC     = 0x01,
    ROCCAT_KONE_XP_MODE_BLINKING   = 0x02,
    ROCCAT_KONE_XP_MODE_BREATHING  = 0x03,
    ROCCAT_KONE_XP_MODE_HEARTBEAT  = 0x04,
    ROCCAT_KONE_XP_MODE_PHOTON_FX  = 0x05,
    ROCCAT_KONE_XP_MODE_DEFAULT    = 0x09,
    ROCCAT_KONE_XP_MODE_WAVE       = 0x0A
};

enum
{
    ROCCAT_KONE_XP_SPEED_MIN             = 0x0B,
    ROCCAT_KONE_XP_SPEED_MAX             = 0x01,
    ROCCAT_KONE_XP_SPEED_DEFAULT         = 0x06,
    ROCCAT_KONE_XP_BRIGHTNESS_MIN        = 0x00,
    ROCCAT_KONE_XP_BRIGHTNESS_MAX        = 0xFF,
    ROCCAT_KONE_XP_BRIGHTNESS_DEFAULT    = 0xFF
};

struct roccat_kone_xp_color_struct
{
    uint8_t     byte_0 = 0x14;
    uint8_t     brightness = 0xFF;
    RGBColor    color = 0;
    uint8_t     byte_5 = 0x64;
};

struct roccat_kone_xp_mode_struct
{
    uint8_t     profile = 0;
    uint8_t     byte_3 = 0x06;
    uint8_t     byte_4 = 0x06;
    uint8_t     dpi_flag = 0x1F;
    uint8_t     byte_6 = 0x01;
    uint16_t    dpi[10] = { 0x08, 0x10, 0x18, 0x20, 0x40, 0x08, 0x10, 0x18, 0x20, 0x40 };
    bool        angle_snapping = false;
    uint8_t     byte_28 = 0x00;
    uint8_t     polling_rate = 0x03;
    uint8_t     mode = ROCCAT_KONE_XP_MODE_STATIC;
    uint8_t     speed = 0x00;
    uint8_t     brightness = 0xFF;
    uint8_t     time_until_idle = 0x0F;
    uint8_t     idle_mode = 0x00;
    uint8_t     byte_35 = 0x00;
    roccat_kone_xp_color_struct colors[20];
    uint8_t     byte_156 = 0x01;
    uint8_t     byte_157 = 0x64;
    uint8_t     profile_color_brightness = 0xFF;
    uint8_t     profile_color_red = 0xFF;
    uint8_t     profile_color_green = 0x00;
    uint8_t     profile_color_blue = 0x00;
    uint8_t     byte_162 = 0x00;
    uint8_t     theme = 0x80;
    uint8_t     auto_dpi_flag = 0x00;
    uint8_t     end_bytes[7] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
};

class RoccatKoneXPController
{
public:
    RoccatKoneXPController(hid_device* dev_handle, char *path, std::string dev_name);
    ~RoccatKoneXPController();

    std::string                 GetLocation();
    std::string                 GetName();
    std::string                 GetSerial();
    std::string                 GetVersion();

    uint8_t                     GetActiveProfile();
    roccat_kone_xp_mode_struct  GetMode();

    void                        EnableDirect(bool on_off_switch);
    void                        SetReadProfile(uint8_t profile);
    void                        SendDirect(std::vector<RGBColor> colors);
    void                        SetMode(roccat_kone_xp_mode_struct mode);

    void                        WaitUntilReady();

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
};

/*---------------------------------------------------------*\
| LogitechG910Controller.h                                  |
|                                                           |
|   Driver for Logitech G910 Orion Spectrum                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                11 Jun 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    LOGITECH_G910_ZONE_MODE_KEYBOARD     = 0x00,
    LOGITECH_G910_ZONE_MODE_LOGO         = 0x01,
};

enum
{
    LOGITECH_G910_ZONE_DIRECT_KEYBOARD   = 0x01,
    LOGITECH_G910_ZONE_DIRECT_GKEYS      = 0x04,
    LOGITECH_G910_ZONE_DIRECT_LOGO       = 0x10,
    LOGITECH_G910_ZONE_DIRECT_INDICATORS = 0x40,
};

enum
{
    LOGITECH_G910_MODE_OFF               = 0x00,
    LOGITECH_G910_MODE_STATIC            = 0x01,
    LOGITECH_G910_MODE_BREATHING         = 0x02,
    LOGITECH_G910_MODE_CYCLE             = 0x03,
    LOGITECH_G910_MODE_WAVE              = 0x04,
};

/*---------------------------------------------------------------------------------------------*\
| Speed is 1000 for fast and 20000 for slow.                                                    |
| Values are multiplied by 100 later to give lots of GUI steps.                                 |
\*---------------------------------------------------------------------------------------------*/
enum
{
    LOGITECH_G910_SPEED_SLOWEST          = 0xC8,     /* Slowest speed                       */
    LOGITECH_G910_SPEED_NORMAL           = 0x32,     /* Normal speed                        */
    LOGITECH_G910_SPEED_FASTEST          = 0x0A,     /* Fastest speed                       */
};

class LogitechG910Controller
{
public:
    LogitechG910Controller(hid_device* dev_handle_0x11, hid_device* dev_handle_0x12, std::string dev_name);
    ~LogitechG910Controller();

    std::string GetNameString();
    std::string GetSerialString();

    void        Commit();

    void        SetDirect
                    (
                    unsigned char       zone,
                    unsigned char       frame_count,
                    unsigned char *     frame_data
                    );

    void        SetMode
                    (
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

private:
    hid_device* dev_pkt_0x11;
    hid_device* dev_pkt_0x12;
    std::string name;

    void        SendDirectFrame
                    (
                    unsigned char       zone,
                    unsigned char       frame_count,
                    unsigned char *     frame_data
                    );

    void        SendMode
                    (
                    unsigned char       zone,
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

    void        SendCommit();
};

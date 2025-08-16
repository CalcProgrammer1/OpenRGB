/*---------------------------------------------------------*\
| LogitechG203LController.h                                 |
|                                                           |
|   Driver for Logitech G203L                               |
|                                                           |
|   Eric Samuelson (edbgon)                     06 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi.h>

#pragma once

enum
{
    LOGITECH_G213_ZONE_MODE_KEYBOARD     = 0x00
};

enum
{
    LOGITECH_G213_MODE_OFF               = 0x00,
    LOGITECH_G213_MODE_STATIC            = 0x01,
    LOGITECH_G213_MODE_BREATHING         = 0x02,
    LOGITECH_G213_MODE_CYCLE             = 0x03,
    LOGITECH_G213_MODE_WAVE              = 0x04,
};

enum
{
    LOGITECH_G213_WAVE_MODE_LEFT         = 0x06,
    LOGITECH_G213_WAVE_MODE_RIGHT        = 0x01,
    LOGITECH_G213_WAVE_MODE_CENTER_EDGE  = 0x03,
    LOGITECH_G213_WAVE_MODE_EDGE_CENTER  = 0x08,
};

/*---------------------------------------------------------------------------------------------*\
| Speed is 1000 for fast and 20000 for slow.                                                    |
| Values are multiplied by 100 later to give lots of GUI steps.                                 |
\*---------------------------------------------------------------------------------------------*/
enum
{
    LOGITECH_G213_SPEED_SLOWEST          = 0xC8,     /* Slowest speed                       */
    LOGITECH_G213_SPEED_NORMAL           = 0x32,     /* Normal speed                        */
    LOGITECH_G213_SPEED_FASTEST          = 0x0A,     /* Fastest speed                       */
};

class LogitechG213Controller
{
public:
    LogitechG213Controller(hid_device* dev_handle, const char* path, std::string dev_name);
    ~LogitechG213Controller();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void        SetDirect
                    (
                    unsigned char       zone,
                    unsigned char       r,
                    unsigned char       g,
                    unsigned char       b
                    );

    void        SetMode
                    (
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned char       direction,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

private:
    hid_device* dev;
    std::string location;
    std::string name;

    void        SendMode
                    (
                    unsigned char       zone,
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned char       direction,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );
};

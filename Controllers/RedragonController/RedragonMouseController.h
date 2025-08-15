/*---------------------------------------------------------*\
| RedragonMouseController.h                                 |
|                                                           |
|   Driver for Redragon mouse                               |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Mar 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

#define REDRAGON_MOUSE_REPORT_ID   0x02
#define REDRAGON_MOUSE_REPORT_SIZE 16
#define REDRAGON_MOUSE_LED_COUNT   1

enum
{
    REDRAGON_MOUSE_MODE_WAVE              = 0x00,
    REDRAGON_MOUSE_MODE_RANDOM_BREATHING  = 0x01,
    REDRAGON_MOUSE_MODE_STATIC            = 0x02,
    REDRAGON_MOUSE_MODE_BREATHING         = 0x04,
    REDRAGON_MOUSE_MODE_RAINBOW           = 0x08,
    REDRAGON_MOUSE_MODE_FLASHING          = 0x10
};

class RedragonMouseController
{
public:
    RedragonMouseController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~RedragonMouseController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void        SendMouseApply();

    void        SendMouseColor
                    (
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

    void        SendMouseMode
                    (
                    unsigned char       mode,
                    unsigned char       speed
                    );

    void        SendMouseMode
                    (
                    unsigned char       mode,
                    unsigned char       speed,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

private:
    hid_device* dev;
    std::string location;
    std::string name;

    void        SendWritePacket
                    (
                    unsigned short      address,
                    unsigned char       data_size,
                    unsigned char *     data
                    );
};

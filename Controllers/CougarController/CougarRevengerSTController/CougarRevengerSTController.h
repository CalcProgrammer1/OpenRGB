/*---------------------------------------------------------*\
| CougarRevengerSTController.h                              |
|                                                           |
|   Driver for Cougar Revenger ST                           |
|                                                           |
|   Morgan Guimard (morg)                       17 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <map>
#include <string>
#include <hidapi.h>

#define PACKET_DATA_LENGTH 9

enum
{
    PACKET_START            = 0xC4,
    ACTION_SET              = 0x0F
};

enum
{
    MIN_BRIGHTNESS          = 0x00,
    MAX_BRIGHTNESS          = 0xFF,
    MIN_SPEED_SWIFT         = 0x2F,
    MIN_SPEED               = 0x0F,
    MAX_SPEED               = 0x00,
    COLORS_SIZE             = 0x07
};

enum
{
    OFF_MODE_VALUE          = 0x00,
    DIRECT_MODE_VALUE       = 0x01,
    BREATHING_MODE_VALUE    = 0x02,
    FLOW_MODE_VALUE         = 0x03,
    SWIFT_MODE_VALUE        = 0x04,
    FLOW_LEFT_MODE_VALUE    = 0x09,
    FLOW_RIGHT_MODE_VALUE   = 0x0B
};

typedef struct
{
    unsigned char zone_mode_mapping[3];
    unsigned char zone_mode_byte[3];
    unsigned char zone_rgb_mapping[3];
    unsigned char zone_rgb_byte[3];
    unsigned char zone_brightness_mapping[3];
    unsigned char zone_brightness_byte[3];
    unsigned char zone_speed_mapping[3];
    unsigned char zone_speed_byte[3];
} cougar_mode;

const cougar_mode DIRECT_MODE =
{
    {
        0x77, 0x55, 0xE6
    },
    {
        0x01, 0x00, 0x00
    },
    {
        0x79, 0x57, 0xE8
    },
    {
        0x01, 0x00, 0x00
    },
    {
        0x78, 0x56, 0xE7
    },
    {
        0x01, 0x00, 0x00
    },
    {
        0x00, 0x00, 0x00
    },
    {
        0x01, 0x00, 0x00
    }
};

const cougar_mode OFF_MODE =
{
    {
        0x77, 0x55, 0xE6
    },
    {
        0x01, 0x00, 0x00
    },
    {
        0x00, 0x00, 0x00
    },
    {
        0x00, 0x00, 0x00
    },
    {
        0x00, 0x00, 0x00
    },
    {
        0x00, 0x00, 0x00
    },
    {
        0x00, 0x00, 0x00
    },
    {
        0x00, 0x00, 0x00
    }
};

const cougar_mode BREATHING_MODE =
{
    {
        0x77, 0x55, 0xE6
    },
    {
        0x01, 0x00, 0x00
    },
    {
        0x85, 0x63, 0xF4
    },
    {
        0x01, 0x00, 0x00
    },
    {
        0x84, 0x62, 0xF3
    },
    {
        0x01, 0x00, 0x00
    },
    {
        0x7C, 0x5A, 0xEB
    },
    {
        0x01, 0x00, 0x00
    }
};

const cougar_mode SWIFT_MODE =
{
    {
        0x77, 0x55, 0xE6
    },
    {
        0x01, 0x00, 0x00
    },
    {
        0xB0, 0x8E, 0x1F
    },
    {
        0x01, 0x00, 0x01
    },
    {
        0xAF, 0x8D, 0x1E
    },
    {
        0x01, 0x00, 0x01
    },
    {
        0xAD, 0x8B, 0x1C
    },
    {
        0x01, 0x00, 0x01
    }
};

const cougar_mode FLOW_MODE =
{
    {
        0x77, 0x00, 0xE6
    },
    {
        0x01, 0x00, 0x00
    },
    {
        0xB0, 0x00, 0x1F
    },
    {
        0x01, 0x00, 0x01
    },
    {
        0xAF, 0x00, 0x1E
    },
    {
        0x01, 0x00, 0x01
    },
    {
        0xAD, 0x00, 0x1C
    },
    {
        0x01, 0x00, 0x01
    }
};

static const std::map<unsigned char, cougar_mode> modes_mapping =
{
    {
        DIRECT_MODE_VALUE,
        DIRECT_MODE
    },
    {
        OFF_MODE_VALUE,
        OFF_MODE
    },
    {
        BREATHING_MODE_VALUE,
        BREATHING_MODE
    },
    {
        SWIFT_MODE_VALUE,
        SWIFT_MODE
    },
    {
        FLOW_LEFT_MODE_VALUE,
        FLOW_MODE
    },
    {
        FLOW_RIGHT_MODE_VALUE,
        FLOW_MODE
    }
};

class CougarRevengerSTController
{
public:
    CougarRevengerSTController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~CougarRevengerSTController();

    std::string     GetSerialString();
    std::string     GetDeviceLocation();
    std::string     GetFirmwareVersion();
    std::string     GetNameString();

    void            ActivateMode(unsigned char zone, unsigned char mode_value);
    void            SetDirect(unsigned char zone, RGBColor color, unsigned char brightness);
    void            SetModeData(unsigned char zone, unsigned char mode_value, std::vector<RGBColor> colors, unsigned char brightness, unsigned char speed);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
    std::string     version;

    void            Apply();
    void            SendColourPacket(unsigned char address, unsigned char value, unsigned char * buffer);
};

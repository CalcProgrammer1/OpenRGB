/*---------------------------------------------------------*\
| ZETEdgeAirProController.h                                 |
|                                                           |
|   Driver for ZET Edge Air Pro                             |
|                                                           |
|   Morgan Guimard (morg)                       29 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define ZET_EDGE_AIR_PRO_PACKET_SIZE    17
#define ZET_EDGE_AIR_PRO_REPORT_ID      0x08
#define ZET_EDGE_AIR_PRO_PACKET_END     0x4a

enum
{
    STATIC_MODE_VALUE       = 0x02,
    OFF_MODE_VALUE          = 0x04,
    RAINBOW_MODE_VALUE      = 0x00,
    BREATHING_MODE_VALUE    = 0x01
};

enum
{
    ZET_EDGE_AIR_PRO_BRIGHTNESS_MIN     = 1,
    ZET_EDGE_AIR_PRO_BRIGHTNESS_MAX     = 10,
    ZET_EDGE_AIR_PRO_SPEED_MIN          = 1,
    ZET_EDGE_AIR_PRO_SPEED_MAX          = 10
};

class ZETEdgeAirProController
{
public:
    ZETEdgeAirProController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~ZETEdgeAirProController();

    std::string                 GetDeviceLocation();
    std::string                 GetNameString();
    std::string                 GetSerialString();

    void                        SetMode(RGBColor color, unsigned char brightness, unsigned char speed, unsigned char mode_value);

protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 name;
};

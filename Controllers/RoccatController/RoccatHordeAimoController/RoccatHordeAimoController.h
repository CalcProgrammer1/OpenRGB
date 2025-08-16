/*---------------------------------------------------------*\
| RoccatHordeAimoController.h                               |
|                                                           |
|   Driver for Roccat Horde Aimo                            |
|                                                           |
|   Morgan Guimard (morg)                       24 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

#define WRITE_PACKET_LENGTH 24
#define READ_PACKET_LENGTH  3
#define REPORT_ID           0x18
#define NUMBER_OF_LEDS      6

class RoccatHordeAimoController
{
public:
    RoccatHordeAimoController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~RoccatHordeAimoController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            SetColors(std::vector<RGBColor> colors);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;

    void            InitialPacket();
};

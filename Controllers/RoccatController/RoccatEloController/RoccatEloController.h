/*---------------------------------------------------------*\
| RoccatEloController.h                                     |
|                                                           |
|   Driver for Roccat Elo                                   |
|                                                           |
|   Flora Aubry                                 02 Jan 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define ROCCAT_ELO_REPORT_SIZE  16
#define ROCCAT_ELO_LEDS_COUNT   1
#define ROCCAT_ELO_REPORT_ID    0xFF

class RoccatEloController
{
public:
    RoccatEloController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~RoccatEloController();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            SendDirect(RGBColor color);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;

    void            SendInit();
};

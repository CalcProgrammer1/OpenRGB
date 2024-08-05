/*---------------------------------------------------------*\
| CorsairK95PlatinumXTController.h                          |
|                                                           |
|   Driver for Corsair K95 Platinum XT keyboard             |
|                                                           |
|   Morgan Guimard (morg)                       07 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string.h>
#include <hidapi.h>
#include "RGBController.h"

#define K95_PLATINUM_XT_REPORT_LENGTH           65
#define K95_PLATINUM_XT_COLOR_DATA_LENGTH       488
#define K95_PLATINUM_XT_REPORT_ID               0x08
#define K95_PLATINUM_XT_NUMBER_OF_PAGES         0x08
#define K95_PLATINUM_XT_READ_COMMAND            0x02
#define K95_PLATINUM_XT_DIRECT_START_STREAM     0x07
#define K95_PLATINUM_XT_DIRECT_BLOCK_STREAM     0x06

class CorsairK95PlatinumXTController
{
public:
    CorsairK95PlatinumXTController(hid_device* dev_handle, const char* path);
    ~CorsairK95PlatinumXTController();

    std::string     GetDeviceLocation();
    std::string     GetFirmwareString();
    std::string     GetName();
    std::string     GetSerialString();

    void            SendDirect(const std::vector<RGBColor>& colors, const std::vector<std::tuple<std::string,unsigned int>>& leds_positions);

private:
    hid_device*     dev;

    std::string     firmware_version;
    std::string     location;
    std::string     name;

    void            Init();
};

/*---------------------------------------------------------*\
| AirgooLustrousCommanderController.h                       |
|                                                           |
|   Driver for Airgoo Lustrous Commander                    |
|                                                           |
|   Zacahry Guinn                               07 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define AIRGOO_LUSTROUS_COMMANDER_PACKET_SIZE       65
#define AIRGOO_LUSTROUS_COMMANDER_NUM_RGB_CHANNELS  2
#define AIRGOO_LUSTROUS_COMMANDER_NUM_FAN_CHANNELS  4

class AirgooLustrousCommanderController
{
public:
    AirgooLustrousCommanderController(hid_device* dev_handle, const char* path);
    ~AirgooLustrousCommanderController();

    std::string GetLocationString();

    void        SetFanMode();
    void        UpdateDevice
                (
                unsigned char   mode,
                unsigned char   red,
                unsigned char   grn,
                unsigned char   blu,
                unsigned char   speed
                );

private:
    hid_device*             dev;
    std::string             location;

    void        InitController();
};

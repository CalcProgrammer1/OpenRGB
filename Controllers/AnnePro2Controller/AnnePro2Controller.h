/*---------------------------------------------------------*\
| AnnePro2Controller.h                                      |
|                                                           |
|   Driver for Obins Lab AnnePro2 keyboard                  |
|                                                           |
|   Sergey Gavrilov (DrZlo13)                   06 Jun 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

class AnnePro2Controller
{
public:
    AnnePro2Controller(hid_device* dev_handle, const char* path);
    ~AnnePro2Controller();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SendDirect
        (
        unsigned char       frame_count,
        unsigned char *     frame_data
        );

private:
    hid_device*                 dev;
    std::string                 location;
};

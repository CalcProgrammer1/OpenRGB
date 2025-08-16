/*---------------------------------------------------------*\
| AsusAuraKeyboardController.h                              |
|                                                           |
|   Driver for ASUS Aura keyboard                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Mar 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

class AuraKeyboardController
{
public:
    AuraKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name);
    virtual ~AuraKeyboardController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void        SendDirect
                    (
                    unsigned char       frame_count,
                    unsigned char *     frame_data
                    );

private:
    hid_device* dev;
    std::string location;
    std::string name;
};

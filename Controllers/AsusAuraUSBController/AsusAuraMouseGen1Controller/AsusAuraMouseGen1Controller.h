/*---------------------------------------------------------*\
| AsusAuraMouseGen1Controller.h                             |
|                                                           |
|   Driver for ASUS Aura gen 1 mouse                        |
|                                                           |
|   Mola19                                      30 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define HID_MAX_STR 255

class AsusAuraMouseGen1Controller
{
public:
    AsusAuraMouseGen1Controller(hid_device* dev_handle, const char* path, uint16_t pid, std::string dev_name);
    virtual ~AsusAuraMouseGen1Controller();

    std::string GetDeviceLocation();
    std::string GetName();
    std::string GetSerialString();
    std::string GetVersion();

    int         GetActiveProfile();

    void        SendUpdate
                    (
                    unsigned char   key,
                    unsigned char   value
                    );

    void        UpdateProfile
                    (
                    unsigned char   key,
                    unsigned char   profile,
                    unsigned char   value
                    );

    void        SendDirectSpatha(std::vector<RGBColor> colors);

    void        ResetToSavedLighting();

    uint16_t    device_pid;

private:
    hid_device* dev;
    std::string location;
    std::string name;
};

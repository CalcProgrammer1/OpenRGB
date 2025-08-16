/*---------------------------------------------------------*\
| AsusAuraMouseController.h                                 |
|                                                           |
|   Driver for ASUS Aura mouse                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                23 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "AsusAuraMouseDevices.h"

#define ASUS_AURA_MOUSE_PACKET_SIZE 65

class AuraMouseController
{
public:
    AuraMouseController(hid_device* dev_handle, const char* path, uint16_t pid, std::string dev_name);
    virtual ~AuraMouseController();

    std::string GetDeviceLocation();
    std::string GetName();
    std::string GetSerialString();
    std::string GetVersion(bool wireless, int protocol);

    std::string CleanSerial(const std::wstring& wstr);

    void        SaveMode();
    void        SendUpdate
                    (
                    unsigned char   zone,
                    unsigned char   mode,
                    unsigned char   red,
                    unsigned char   grn,
                    unsigned char   blu,
                    unsigned char   dir,
                    bool            random,
                    unsigned char   speed,
                    unsigned char   brightness
                    );
    void        SendDirect
                    (
                    std::vector<RGBColor>   zone_colors
                    );

    uint16_t    device_pid;

private:
    hid_device* dev;
    std::string location;
    std::string name;
};

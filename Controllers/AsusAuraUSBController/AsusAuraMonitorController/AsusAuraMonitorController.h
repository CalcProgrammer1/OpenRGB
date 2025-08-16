/*---------------------------------------------------------*\
| AsusAuraMonitorController.h                               |
|                                                           |
|   Driver for ASUS Aura monitor                            |
|                                                           |
|   Mola19                                      08 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

enum
{
    AURA_ROG_STRIX_XG27AQ_PID   = 0x198C,
    AURA_ROG_STRIX_XG27AQM_PID  = 0x19BB,
    AURA_ROG_STRIX_XG279Q_PID   = 0x1919,
    AURA_ROG_STRIX_XG27W_PID    = 0x1933,
    AURA_ROG_STRIX_XG32VC_PID   = 0x1968,
    AURA_ROG_PG32UQ_PID         = 0x19B9,
};

class AuraMonitorController
{
public:
    AuraMonitorController(hid_device* dev_handle, const char* path, uint16_t pid, std::string dev_name);
    virtual ~AuraMonitorController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void        BeginUpdate();
    void        UpdateLed(int led, unsigned char red, unsigned char green, unsigned char blue);
    void        ApplyChanges();

    uint16_t    device_pid;

private:
    hid_device* dev;
    std::string location;
    std::string name;
};

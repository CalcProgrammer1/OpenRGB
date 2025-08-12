/*---------------------------------------------------------*\
| LenovoM300Controller.h                                    |
|                                                           |
|   Driver for Lenovo Legion M300 mouse                     |
|                                                           |
|   Wayne Riordan                               09 Jan 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define M300_DATA_SIZE  0x41
#define M300_MAX_BRIGTH 0x64
#define M300_MIN_BRIGHT 0x01

enum
{
    M300_MODE_OFF       = 0x00,
    M300_MODE_RAINBOW   = 0x01,
    M300_MODE_BREATHING = 0x02,
    M300_MODE_STATIC    = 0X03
};

class LenovoM300Controller
{
public:
    LenovoM300Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~LenovoM300Controller();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    void SetMode(std::vector<RGBColor> colors, unsigned char mode_value, unsigned int brightness);

protected:
    hid_device* device;

private:
    std::string     location;
    std::string     name;

    unsigned char CalculateFinalByte(unsigned char* ptr, int count);
};

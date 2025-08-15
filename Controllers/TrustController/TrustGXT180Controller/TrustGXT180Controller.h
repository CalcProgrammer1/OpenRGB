/*---------------------------------------------------------*\
| TrustGXT180Controller.h                                   |
|                                                           |
|   Driver for Trust GXT 180                                |
|                                                           |
|   Morgan Guimard (morg)                       24 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define TRUST_GXT_180_REPORT_SIZE       64
#define TRUST_GXT_180_NUMBER_OF_LEDS    1
#define TRUST_GXT_180_REPORT_ID         0x03

enum
{
    TRUST_GXT_180_STATIC_MODE_VALUE         = 0x01,
    TRUST_GXT_180_BREATHING_MODE_VALUE      = 0x02,
    TRUST_GXT_180_COLORSHIFT_MODE_VALUE     = 0x03,
    TRUST_GXT_180_OFF_MODE_VALUE            = 0x04
};

enum
{
    TRUST_GXT_180_BRIGHTNESS_MIN            = 0x00,
    TRUST_GXT_180_BRIGHTNESS_MAX            = 0x05
};

enum
{
    TRUST_GXT_180_SPEED_MIN                 = 0x0A,
    TRUST_GXT_180_SPEED_MAX                 = 0x00
};

class TrustGXT180Controller
{
public:
    TrustGXT180Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~TrustGXT180Controller();

    std::string                 GetDeviceLocation();
    std::string                 GetNameString();
    std::string                 GetSerialString();

    void                        SetMode(RGBColor color, unsigned char brightness, unsigned char speed, unsigned char mode_value);

protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 name;
};

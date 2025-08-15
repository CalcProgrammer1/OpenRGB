/*---------------------------------------------------------*\
| ZETBladeOpticalController.h                               |
|                                                           |
|   Driver for ZET Blade                                    |
|                                                           |
|   Based on HyperX Alloy Elite2 implementation by          |
|   KundaPanda                                              |
|                                                           |
|   Moon_darker (Vaker)                         23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define ZET_BLADE_OPTICAL_DELAY       12ms
#define ZET_BLADE_OPTICAL_HEADER_LEN     9
#define ZET_BLADE_OPTICAL_COLOR_LEN      4
#define ZET_BLADE_OPTICAL_KEY_OFFSET  0x80

#define ZET_BLADE_OPTICAL_SPEED_MIN      0
#define ZET_BLADE_OPTICAL_SPEED_MAX      4
#define ZET_BLADE_OPTICAL_SPEED_DEF      2
#define ZET_BLADE_OPTICAL_BRIGHTNESS_MIN 0
#define ZET_BLADE_OPTICAL_BRIGHTNESS_MAX 4
#define ZET_BLADE_OPTICAL_BRIGHTNESS_DEF 4

#define ZET_BLADE_OPTICAL_MODE_OFF            0x01
#define ZET_BLADE_OPTICAL_MODE_CUSTOM         0x1C
#define ZET_BLADE_OPTICAL_MODE_STATIC         0x02
#define ZET_BLADE_OPTICAL_MODE_BREATHING      0x03
#define ZET_BLADE_OPTICAL_MODE_ON_PRESS       0x04
#define ZET_BLADE_OPTICAL_MODE_RAINDROP       0x05
#define ZET_BLADE_OPTICAL_MODE_RIPPLE         0x06
#define ZET_BLADE_OPTICAL_MODE_LASER          0x07
#define ZET_BLADE_OPTICAL_MODE_WAVES          0x08
#define ZET_BLADE_OPTICAL_MODE_RAIN           0x09
#define ZET_BLADE_OPTICAL_MODE_SPECTRUM       0x0A
#define ZET_BLADE_OPTICAL_MODE_SURFING_RIGHT  0x0B
#define ZET_BLADE_OPTICAL_MODE_SURFING_CENTER 0x0D
#define ZET_BLADE_OPTICAL_MODE_SURFING_CROSS  0x0F
#define ZET_BLADE_OPTICAL_MODE_ROTATE_MARQUEE 0x0C
#define ZET_BLADE_OPTICAL_MODE_TRAFFIC        0x0E
#define ZET_BLADE_OPTICAL_MODE_GRADIENT       0x15

class ZETBladeOpticalController
{
public:
    ZETBladeOpticalController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~ZETBladeOpticalController();

    std::string                         GetDeviceLocation();
    std::string                         GetNameString();
    std::string                         GetSerialString();

    void                                SetLEDDirect(const std::vector<RGBColor>& colors, unsigned char brightness);

    void                                SetEffect(unsigned char mode, unsigned char speed, unsigned char brightness, bool random, unsigned char red1, unsigned char grn1, unsigned char blu1);
private:
    hid_device*                         dev;
    std::string                         location;
    std::string                         name;
    unsigned int                        effect_mode;
    bool                                custom_mode;

    void                                PrepareHeader(unsigned char *packet, unsigned char brightness);
    void                                PrepareHeader(unsigned char *packet, unsigned char mode, unsigned char speed, unsigned char brightness, unsigned char color);
    unsigned char                       RGBToPalette(unsigned char red, unsigned char grn, unsigned char blu);
};

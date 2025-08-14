/*---------------------------------------------------------*\
| MSIVigorGK30Controller.h                                  |
|                                                           |
|   Driver for MSI Vigor GK30                               |
|                                                           |
|   Morgan Guimard (morg)                       01 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define MSI_VIGOR_GK30_REPORT_SIZE  8
#define MSI_VIGOR_GK30_LEDS_COUNT   6
#define MSI_VIGOR_GK30_REPORT_ID    0x07

enum
{
    MSI_VIGOR_GK30_OFF_MODE_VALUE       = 0x00,
    MSI_VIGOR_GK30_STATIC_MODE_VALUE    = 0x10,
    MSI_VIGOR_GK30_BREATHING_MODE_VALUE = 0x20,
    MSI_VIGOR_GK30_RAINBOW_MODE_VALUE   = 0x30,
    MSI_VIGOR_GK30_METEOR_MODE_VALUE    = 0x40,
    MSI_VIGOR_GK30_RIPPLE_MODE_VALUE    = 0x50,
    MSI_VIGOR_GK30_DIMMING_MODE_VALUE   = 0x60,
    MSI_VIGOR_GK30_CUSTOM_MODE_VALUE    = 0x70
};

enum
{
    MSI_VIGOR_GK30_BRIGHTNESS_MIN       = 0x01,
    MSI_VIGOR_GK30_BRIGHTNESS_MAX       = 0x03
};

enum
{
    MSI_VIGOR_GK30_SPEED_MIN            = 0x01,
    MSI_VIGOR_GK30_SPEED_MAX            = 0x03
};

class MSIVigorGK30Controller
{
public:
    MSIVigorGK30Controller(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~MSIVigorGK30Controller();

    std::string     GetDeviceLocation();
    std::string     GetNameString();
    std::string     GetSerialString();

    void            SetMode(std::vector<RGBColor> colors,
                            unsigned char brightness,
                            unsigned char speed,
                            unsigned char mode_value,
                            unsigned int mode_flags,
                            unsigned int color_mode,
                            unsigned char direction
                            );

protected:
    hid_device*     dev;

private:

    unsigned int    GetLargestColour(unsigned int red, unsigned int green, unsigned int blue);
    unsigned char   GetColourIndex(unsigned char red, unsigned char green, unsigned char blue);

    std::string     location;
    std::string     name;
    std::string     serial_number;
};

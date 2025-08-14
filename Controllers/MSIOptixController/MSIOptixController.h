/*---------------------------------------------------------*\
| MSIOptixController.h                                      |
|                                                           |
|   Driver for MSI Optix                                    |
|                                                           |
|   Morgan Guimard (morg)                       10 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define MSI_OPTIX_REPORT_SIZE 168
#define MSI_OPTIX_COLOR_PACKET_SIZE 48
#define MSI_OPTIX_NUMBER_OF_LEDS 12
#define MSI_OPTIX_DIRECT_COLOR_OFFSET 24 * 3;
#define MSI_OPTIC_REPORT_ID 0x72
#define MSI_OPTIX_DEFAULT_MODE_COLOR ToRGBColor(255,0,0)

enum
{
    OFF_MODE_VALUE              = 0x00,
    RAINBOW_MODE_VALUE          = 0x0f,
    METEOR_MODE_VALUE           = 0x07,
    STACK_MODE_VALUE            = 0x08,
    BREATHING_MODE_VALUE        = 0x02,
    FLASHING_MODE_VALUE         = 0x03,
    DOUBLE_FLASHING_MODE_VALUE  = 0x04,
    DIRECT_MODE_VALUE           = 0x01,
    STATIC_MODE_VALUE           = 0x01,
    LIGHNING_MODE_VALUE         = 0x05,
    PLANETARY_MODE_VALUE        = 0x10,
    DOUBLE_METEOR_MODE_VALUE    = 0x11,
    ENERGY_MODE_VALUE           = 0x12,
    BLINK_MODE_VALUE            = 0x13,
    CLOCK_MODE_VALUE            = 0x14,
    COLOR_PULSE_MODE_VALUE      = 0x15,
    COLOR_SHIFT_MODE_VALUE      = 0x16,
    COLOR_WAVE_MODE_VALUE       = 0x17,
    MARQUEE_MODE_VALUE          = 0x18,
    RAINBOW_WAVE_MODE_VALUE     = 0x1a,
    VISOR_MODE_VALUE            = 0x1b
};

enum
{
    MSI_OPTIX_BRIGHTNESS_MIN    = 0x00,
    MSI_OPTIX_BRIGHTNESS_MAX    = 0x64
};

enum
{
    MSI_OPTIX_SPEED_MIN         = 0x00,
    MSI_OPTIX_SPEED_MAX         = 0x02
};

enum
{
    MSI_OPTIX_MYSTERIOUS_FLAG   = 0x80
};

class MSIOptixController
{
public:
    MSIOptixController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~MSIOptixController();

    std::string                 GetDeviceLocation();
    std::string                 GetNameString();
    std::string                 GetSerialString();

    void                        SetDirect(std::vector<RGBColor> colors, unsigned char brightness);
    void                        SetMode(std::vector<RGBColor> colors, unsigned char brightness, unsigned char speed, unsigned char mode_value, unsigned int mode_flags);

protected:
    hid_device* dev;

private:
    std::string                 location;
    std::string                 name;

    unsigned char               GetMysteriousFlag(unsigned char mode_value);
};

/*---------------------------------------------------------*\
| GigabyteAorusLaptopController.h                           |
|                                                           |
|   Driver for Gigabyte Aorus laptop                        |
|                                                           |
|   Morgan Guimard (morg)                       05 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define GIGABYTE_AORUS_LAPTOP_REPORT_SIZE      8
#define GIGABYTE_AORUS_LAPTOP_REPORT_ID        0x08

enum
{
    GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MIN                = 0x00,
    GIGABYTE_AORUS_LAPTOP_BRIGHTNESS_MAX                = 0x32,
    GIGABYTE_AORUS_LAPTOP_SPEED_MIN                     = 0x01,
    GIGABYTE_AORUS_LAPTOP_SPEED_MAX                     = 0x09
};

enum
{
    GIGABYTE_AORUS_LAPTOP_DIRECT_MODE_VALUE             = 0x00,
    GIGABYTE_AORUS_LAPTOP_STATIC_MODE_VALUE             = 0x01,
    GIGABYTE_AORUS_LAPTOP_PULSE_MODE_VALUE              = 0x02,
    GIGABYTE_AORUS_LAPTOP_WAVE_MODE_VALUE               = 0x03,
    GIGABYTE_AORUS_LAPTOP_REACTIVE_MODE_VALUE           = 0x04,
    GIGABYTE_AORUS_LAPTOP_MARQUEE_MODE_VALUE            = 0x05,
    GIGABYTE_AORUS_LAPTOP_RIPPLE_MODE_VALUE             = 0x06,
    GIGABYTE_AORUS_LAPTOP_CYCLE_MODE_VALUE              = 0x08,
    GIGABYTE_AORUS_LAPTOP_RAINBOW_MARQUEE_MODE_VALUE    = 0x09,
    GIGABYTE_AORUS_LAPTOP_DROPLET_MODE_VALUE            = 0x0A,
    GIGABYTE_AORUS_LAPTOP_CIRCLE_MARQUEE_MODE_VALUE     = 0x0B,
    GIGABYTE_AORUS_LAPTOP_HEDGE_MODE_VALUE              = 0x0C,
    GIGABYTE_AORUS_LAPTOP_SPIRAL_MODE_VALUE             = 0x0D,
    GIGABYTE_AORUS_LAPTOP_CURTAIN_MODE_VALUE            = 0x40,
    GIGABYTE_AORUS_LAPTOP_COMET_MODE_VALUE              = 0x41,
    GIGABYTE_AORUS_LAPTOP_CHASE_MODE_VALUE              = 0x43,
    GIGABYTE_AORUS_LAPTOP_CUSTOM_MODE_VALUE             = 0x33
};

class GigabyteAorusLaptopController
{
public:
    GigabyteAorusLaptopController(hid_device* dev_handle, const hid_device_info& info, std::string dev_name);
    ~GigabyteAorusLaptopController();

    std::string GetNameString();
    std::string GetSerialString();
    std::string GetDeviceLocation();

    void        SetDirect(uint8_t brightness, RGBColor color);
    void        SetMode(uint8_t mode_value, uint8_t speed,  uint8_t brightness, uint8_t direction, RGBColor color);
    void        SetCustom(std::vector<RGBColor> colors, std::vector<unsigned int> positions, unsigned char brightness);

protected:
    hid_device* dev;

private:
    unsigned int    GetLargestColour(unsigned int red, unsigned int green, unsigned int blue);
    unsigned char   GetColourIndex(unsigned char red, unsigned char green, unsigned char blue);

    std::string location;
    std::string name;
};

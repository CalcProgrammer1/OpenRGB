/*---------------------------------------------------------*\
| LightSaltController.h                                     |
|                                                           |
|   Driver for LightSalt Peripherals                        |
|                                                           |
|   James Buren (braewoods)                     23 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define LIGHTSALT_PACKET_LENGTH 32
#define LIGHTSALT_WRITE_LENGTH  (LIGHTSALT_PACKET_LENGTH + 1)
#define LIGHTSALT_READ_LENGTH   (LIGHTSALT_PACKET_LENGTH)

enum
{
    LIGHTSALT_TYPE_KEYBOARD,
    LIGHTSALT_TYPE_KEYPAD,
    LIGHTSALT_TYPE_MAXIMUM
};

enum
{
    LIGHTSALT_MODE_CUSTOM,
    LIGHTSALT_MODE_POINT,
    LIGHTSALT_MODE_AREA,
    LIGHTSALT_MODE_COLLAPSE,
    LIGHTSALT_MODE_EXPAND,
    LIGHTSALT_MODE_EXPLODE,
    LIGHTSALT_MODE_DART,
    LIGHTSALT_MODE_FLAME,
    LIGHTSALT_MODE_LASER,
    LIGHTSALT_MODE_BREATHING,
    LIGHTSALT_MODE_TRICOLOR_RADAR,
    LIGHTSALT_MODE_WHEEL_1,
    LIGHTSALT_MODE_WHEEL_2,
    LIGHTSALT_MODE_WAVE_1,
    LIGHTSALT_MODE_WAVE_2,
    LIGHTSALT_MODE_WAVE_3,
    LIGHTSALT_MODE_RAINBOW_1,
    LIGHTSALT_MODE_RAINBOW_2,
    LIGHTSALT_MODE_MAXIMUM
};

enum
{
    LIGHTSALT_SETS_MAX    =  4,
    LIGHTSALT_ROWS_MAX    =  9,
    LIGHTSALT_COLUMNS_MAX = 10
};

enum
{
    LIGHTSALT_SPEED_MINIMUM = 0x01,
    LIGHTSALT_SPEED_MAXIMUM = 0x32,
    LIGHTSALT_SPEED_DEFAULT = 0x19
};

enum
{
    LIGHTSALT_BRIGHTNESS_MINIMUM = 0x00,
    LIGHTSALT_BRIGHTNESS_MAXIMUM = 0xFF,
    LIGHTSALT_BRIGHTNESS_DEFAULT = 0x80
};

class LightSaltController
{
public:
    LightSaltController(hid_device* dev_handle, const hid_device_info& info);
    ~LightSaltController();

    std::string GetDeviceLocation();
    std::string GetSerial();
    std::string GetManufacturer();
    std::string GetProduct();
    std::string GetDeviceModel();
    std::string GetDeviceClass();
    std::string GetFirmwareVersion();
    int GetDeviceType();

    void SetColors(RGBColor* colors, int sets, int rows, int columns);
    void ApplyColors(int sets);
    void SaveColors(int sets);
    void SetMode(int mode, int direction, int speed);
    void SetFilter(RGBColor color);
    void SetBrightness(uint8_t brightness);

private:
    hid_device* dev;

    std::string device_location;
    std::string manufacturer;
    std::string product;
    std::string device_model;
    std::string device_class;
    std::string firmware_version;
    int device_type;

    void QueryDeviceModel();
    void QueryDeviceClass();
    void SetDeviceType();
};

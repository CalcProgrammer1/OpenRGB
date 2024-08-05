/*---------------------------------------------------------*\
| TecknetController.h                                       |
|                                                           |
|   Driver for Tecknet devices                              |
|                                                           |
|   Chris M (Dr_No)                             29 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

#define HID_MAX_STR                   255
#define TECKNET_COLOUR_MODE_DATA_SIZE (sizeof(tecknet_colour_mode_data[0]) / sizeof(tecknet_colour_mode_data[0][0]))
#define TECKNET_DEVICE_NAME_SIZE      (sizeof(device_name) / sizeof(device_name[ 0 ]))
#define TECKNET_PACKET_LENGTH         0x10                  //16 bytes

enum
{
    TECKNET_RED_BYTE            = 2,
    TECKNET_GREEN_BYTE          = 3,
    TECKNET_BLUE_BYTE           = 4,
    TECKNET_BRIGHTNESS_BYTE     = 5,
    TECKNET_SPEED_BYTE          = 6
};

enum
{
    TECKNET_MODE_OFF            = 0xFF,                     //LEDs Off
    TECKNET_MODE_DIRECT         = 0x00,                     //Direct Mode
    TECKNET_MODE_BREATHING      = 0x01,                     //Breathing Mode
};

enum
{
    TECKNET_BRIGHTNESS_OFF      = 0x00,
    TECKNET_BRIGHTNESS_LOW      = 0x01,
    TECKNET_BRIGHTNESS_MED      = 0x02,
    TECKNET_BRIGHTNESS_HIGH     = 0x03
};

enum
{
    TECKNET_SPEED_OFF             = 0x00,                   // Breathe Off
    TECKNET_SPEED_SLOW            = 0x01,                   // Breathe Slow speed
    TECKNET_SPEED_NORMAL          = 0x02,                   // Breathe Normal speed
    TECKNET_SPEED_FAST            = 0x03,                   // Breathe Fast speed
};

class TecknetController
{
public:
    TecknetController(hid_device *dev_handle, char *_path);
    ~TecknetController();

    std::string GetDeviceName();
    std::string GetSerial();
    std::string GetLocation();

    void SetMode(unsigned char mode, unsigned char speed, unsigned char brightness);
    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    std::string             device_name;
    std::string             location;
    hid_device*             dev;

    unsigned char           current_mode;
    unsigned char           current_speed;
    unsigned char           current_brightness;

    unsigned char           current_red;
    unsigned char           current_green;
    unsigned char           current_blue;

    void SendUpdate();
};

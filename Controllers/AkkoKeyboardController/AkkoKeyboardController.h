/*---------------------------------------------------------*\
| AkkoKeyboardController.h                                  |
|                                                           |
|   Driver for Akko Multi-modes keyboards                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <hidapi.h>

#define AKKO_REPORT_LENGTH              64
#define AKKO_HID_MAX_STR                255

#define AKKO_COMMAND_SET_LED_PARAMS     0x07
#define AKKO_COMMAND_GET_LED_PARAMS     0x87

#define AKKO_EFFECT_OFF                 0x00
#define AKKO_EFFECT_STATIC              0x01
#define AKKO_EFFECT_BREATHING           0x02
#define AKKO_EFFECT_WAVE                0x04
#define AKKO_EFFECT_RIPPLE              0x05
#define AKKO_EFFECT_REACTIVE            0x08

#define AKKO_SPEED_MIN                  0x00
#define AKKO_SPEED_MAX                  0x04
#define AKKO_SPEED_DEFAULT              0x00

#define AKKO_BRIGHTNESS_MIN             0x00
#define AKKO_BRIGHTNESS_MAX             0x04
#define AKKO_BRIGHTNESS_DEFAULT         0x03

#define AKKO_OPTION_RAINBOW             0x07
#define AKKO_OPTION_FIXED_COLOR         0x08

class AkkoKeyboardController
{
public:
    AkkoKeyboardController(hid_device* dev_handle, char* path);
    ~AkkoKeyboardController();

    bool            IsValid() const;

    std::string     GetDeviceName();
    std::string     GetSerial();
    std::string     GetLocation();

    unsigned char   GetMode() const;
    unsigned char   GetSpeed() const;
    unsigned char   GetBrightness() const;
    unsigned char   GetRed() const;
    unsigned char   GetGreen() const;
    unsigned char   GetBlue() const;

    void            SetMode(unsigned char mode, unsigned char speed, unsigned char brightness);
    void            SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    std::string     device_name;
    std::string     location;
    hid_device*     dev;
    bool            valid;

    unsigned char   current_mode;
    unsigned char   current_speed;
    unsigned char   current_brightness;
    unsigned char   current_option;
    unsigned char   current_red;
    unsigned char   current_green;
    unsigned char   current_blue;

    static unsigned char CalculateChecksum(const unsigned char* data, std::size_t length);
    bool            ReadState();
    bool            SendUpdate();
};

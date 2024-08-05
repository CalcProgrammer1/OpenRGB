/*---------------------------------------------------------*\
| EpomakerController.h                                      |
|                                                           |
|   Driver for Epomaker keyboard                            |
|                                                           |
|   Alvaro Munoz (alvaromunoz)                  05 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>

#define EPOMAKER_PACKET_LENGTH                      0x40
#define EPOMAKER_COMMAND_RGB                        0x07
#define EPOMAKER_COMMAND_SET                        0xF60A
#define EPOMAKER_COMMAND_PING                       0xF7
#define HID_MAX_STR                                 255

enum
{
    EPOMAKER_BYTE_COMMAND                           = 1,
    EPOMAKER_BYTE_MODE                              = 2,
    EPOMAKER_BYTE_SPEED                             = 3,
    EPOMAKER_BYTE_BRIGHTNESS                        = 4,
    EPOMAKER_BYTE_FLAGS                             = 5,
    EPOMAKER_BYTE_RED                               = 6,
    EPOMAKER_BYTE_GREEN                             = 7,
    EPOMAKER_BYTE_BLUE                              = 8,
    EPOMAKER_BYTE_FILLER                            = 9
};

enum
{
    EPOMAKER_MODE_ALWAYS_ON                         = 0x01,
    EPOMAKER_MODE_DYNAMIC_BREATHING                 = 0x02,
    EPOMAKER_MODE_SPECTRUM_CYCLE                    = 0x03,
    EPOMAKER_MODE_DRIFT                             = 0x04,
    EPOMAKER_MODE_WAVES_RIPPLE                      = 0x05,
    EPOMAKER_MODE_STARS_TWINKLE                     = 0x06,
    EPOMAKER_MODE_STEADY_STREAM                     = 0x07,
    EPOMAKER_MODE_SHADOWING                         = 0x08,
    EPOMAKER_MODE_PEAKS_RISING_ONE_AFTER_ANOTHER    = 0x09,
    EPOMAKER_MODE_SINE_WAVE                         = 0x0a,
    EPOMAKER_MODE_CAISPRING_SURGING                 = 0x0b,
    EPOMAKER_MODE_FLOWERS_BLOOMING                  = 0x0c,
    EPOMAKER_MODE_LASER                             = 0x0e,
    EPOMAKER_MODE_PEAK_TURN                         = 0x0f,
    EPOMAKER_MODE_INCLINED_RAIN                     = 0x10,
    EPOMAKER_MODE_SNOW                              = 0x11,
    EPOMAKER_MODE_METEOR                            = 0x12,
    EPOMAKER_MODE_THROUGH_THE_SNOW_NON_TRACE        = 0x13,
    EPOMAKER_MODE_LIGHT_SHADOW                      = 0x15
};

enum
{
    EPOMAKER_SPEED_MIN                              = 0x00,
    EPOMAKER_SPEED_MAX                              = 0x05,
    EPOMAKER_SPEED_MAX_SPECIAL                      = 0x04,
    EPOMAKER_SPEED_DEFAULT                          = 0x04
};

enum
{
    EPOMAKER_BRIGHTNESS_MIN                         = 0x00,
    EPOMAKER_BRIGHTNESS_MAX                         = 0x04,
    EPOMAKER_BRIGHTNESS_DEFAULT                     = 0x04
};

enum
{
    EPOMAKER_OPTION_DAZZLE_OFF                      = 0x07,
    EPOMAKER_OPTION_DAZZLE_ON                       = 0x08,
    EPOMAKER_OPTION_DEFAULT                         = 0x00,
    EPOMAKER_OPTION_DRIFT_RIGHT                     = 0X00,
    EPOMAKER_OPTION_DRIFT_LEFT                      = 0X10,
    EPOMAKER_OPTION_DRIFT_DOWN                      = 0X20,
    EPOMAKER_OPTION_DRIFT_UP                        = 0X30,
    EPOMAKER_OPTION_STEADY_STREAM_ZIG_ZAG           = 0x00,
    EPOMAKER_OPTION_STEADY_STREAM_RETURN            = 0x10,
    EPOMAKER_OPTION_CAISPRING_SURGING_OUT           = 0x00,
    EPOMAKER_OPTION_CAISPRING_SURGING_IN            = 0x10,
    EPOMAKER_OPTION_FLOWERS_BLOOMING_RIGHT          = 0x00,
    EPOMAKER_OPTION_FLOWERS_BLOOMING_LEFT           = 0x10,
    EPOMAKER_OPTION_PEAK_TURN_ANTI_CLOCKWISE        = 0x00,
    EPOMAKER_OPTION_PEAK_TURN_CLOCKWISE             = 0x10,
};

class EpomakerController
{
public:
    EpomakerController(hid_device* dev_handle, char *_path);
    ~EpomakerController();

    std::string GetDeviceName();
    std::string GetSerial();
    std::string GetLocation();

    void SetDazzle(bool is_dazzle);
    void SetOption(unsigned char option);

    void SetMode(unsigned char mode, unsigned char speed, unsigned char brightness);
    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    std::string             device_name;
    std::string             location;
    hid_device*             dev;

    unsigned char           current_mode;
    unsigned char           current_speed;
    unsigned char           current_brightness;
    unsigned char           current_dazzle;
    unsigned char           current_option;

    unsigned char           current_red;
    unsigned char           current_green;
    unsigned char           current_blue;

    void SendUpdate();
};

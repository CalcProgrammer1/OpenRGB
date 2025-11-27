/*---------------------------------------------------------*\
| FnaticStreakController.h                                  |
|                                                           |
|   Driver for Fnatic Streak and miniStreak keyboards       |
|                                                           |
|   Based on leddy project by Hanna Czenczek                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*-----------------------------------------------------*\
| Fnatic Streak keyboard layout variants                |
\*-----------------------------------------------------*/
#define FNATIC_STREAK_VARIANT_ISO           0x00
#define FNATIC_STREAK_VARIANT_ANSI          0x01

/*-----------------------------------------------------*\
| Fnatic Streak effect command bytes                    |
\*-----------------------------------------------------*/
#define FNATIC_STREAK_CMD_PULSE             0x06
#define FNATIC_STREAK_CMD_WAVE              0x07
#define FNATIC_STREAK_CMD_REACTIVE          0x09
#define FNATIC_STREAK_CMD_REACTIVE_RIPPLE   0x0A
#define FNATIC_STREAK_CMD_RAIN              0x0B
#define FNATIC_STREAK_CMD_GRADIENT          0x0C
#define FNATIC_STREAK_CMD_FADE              0x0D

/*-----------------------------------------------------*\
| Fnatic Streak color modes                             |
\*-----------------------------------------------------*/
#define FNATIC_STREAK_COLOR_MODE_SINGLE     0x00
#define FNATIC_STREAK_COLOR_MODE_RAINBOW    0x01
#define FNATIC_STREAK_COLOR_MODE_RANDOM     0x02
#define FNATIC_STREAK_COLOR_MODE_GRADIENT   0x03

/*-----------------------------------------------------*\
| Fnatic Streak directions                              |
\*-----------------------------------------------------*/
#define FNATIC_STREAK_DIRECTION_RIGHT       0x01
#define FNATIC_STREAK_DIRECTION_LEFT        0x02
#define FNATIC_STREAK_DIRECTION_DOWN        0x03
#define FNATIC_STREAK_DIRECTION_UP          0x04

enum FnaticStreakType
{
    FNATIC_STREAK_TYPE_FULL,
    FNATIC_STREAK_TYPE_MINI
};

class FnaticStreakController
{
public:
    FnaticStreakController(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name, FnaticStreakType kb_type);
    ~FnaticStreakController();

    std::string         GetDeviceLocation();
    std::string         GetNameString();
    std::string         GetSerialString();
    std::string         GetFirmwareVersion();
    FnaticStreakType    GetKeyboardType();
    unsigned int        GetLEDCount();

    void SetProfile(unsigned char profile);
    void SetLEDsDirect(std::vector<led> leds, std::vector<RGBColor> colors, unsigned int brightness);
    void SendRGBToDevice();

    /*-----------------------------------------------------*\
    | Hardware effect methods                               |
    \*-----------------------------------------------------*/
    void SetPulse(unsigned char color_mode, unsigned char r, unsigned char g, unsigned char b, unsigned char speed);
    void SetWave(unsigned char color_mode, unsigned char r, unsigned char g, unsigned char b, unsigned char speed, unsigned char direction);
    void SetReactive(unsigned char color_mode, unsigned char r, unsigned char g, unsigned char b, unsigned char speed, bool keyup);
    void SetReactiveRipple(unsigned char color_mode, unsigned char r, unsigned char g, unsigned char b, unsigned char speed, bool keyup);
    void SetRain(unsigned char color_mode, unsigned char r, unsigned char g, unsigned char b, unsigned char speed, unsigned char direction);
    void SetGradient(unsigned char colors[][3], unsigned char positions[], unsigned int count);
    void SetFade(unsigned char color_mode, unsigned char colors[][3], unsigned char positions[], unsigned int count, unsigned char speed);

    void SoftwareEffectStart();
    void SoftwareEffectEnd();
    void SendKeepalive();

private:
    void SendRequest(const unsigned char* prefix, size_t prefix_len, const unsigned char* raw_data, size_t data_len);
    void RefreshProfile();

    hid_device*         dev;
    std::string         location;
    std::string         firmware_version;
    std::string         name;
    FnaticStreakType    keyboard_type;
    unsigned char       profile;
    bool                software_effect_mode;

    /*-----------------------------------------------------*\
    | Buffer for LED colors                                 |
    | Full: 124 LEDs * 3 (RGB) = 372 bytes                  |
    | Mini: 106 LEDs * 3 (RGB) = 318 bytes                  |
    | Using max size to support both                        |
    \*-----------------------------------------------------*/
    unsigned char       color_buf[372];
};

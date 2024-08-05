/*---------------------------------------------------------*\
| ThermaltakePoseidonZRGBController.h                       |
|                                                           |
|   Driver for Thermaltake Poseidon Z RGB                   |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define POSEIDONZ_START       0x07
#define POSEIDONZ_PROFILE     0x01
#define POSEIDONZ_LED_CMD     0x0E
#define POSEIDONZ_RED_GRN_CH  0x01
#define POSEIDONZ_BLU_CH      0x02

enum
{
    POSEIDONZ_PACKET_ID_SET_EFFECT      = 0x02,     /* Set profile effect packet            */
    POSEIDONZ_PACKET_ID_SET_COLOR       = 0x09,     /* Set profile color packet             */
    POSEIDONZ_PACKET_ID_SET_DIRECT      = 0x0E,     /* Set direct color packet              */
};

enum
{
    POSEIDONZ_MODE_STATIC       = 0x00,
    POSEIDONZ_MODE_REACTIVE     = 0x01,
    POSEIDONZ_MODE_ARROW_FLOW   = 0x02,
    POSEIDONZ_MODE_WAVE         = 0x03,
    POSEIDONZ_MODE_RIPPLE       = 0x04
};

enum
{
    POSEIDONZ_PROFILE_P1        = 0x01,
    POSEIDONZ_PROFILE_P2        = 0x02,
    POSEIDONZ_PROFILE_P3        = 0x03,
    POSEIDONZ_PROFILE_P4        = 0x04,
    POSEIDONZ_PROFILE_P5        = 0x05
};

enum
{
    POSEIDONZ_BRIGHTNESS_MIN    = 0x00,
    POSEIDONZ_BRIGHTNESS_MAX    = 0x04
};

enum
{
    POSEIDONZ_COLOR_RED         = 0x01,
    POSEIDONZ_COLOR_GREEN       = 0x02,
    POSEIDONZ_COLOR_BLUE        = 0x03
};

enum
{
    POSEIDONZ_DIRECT_RED_GREEN  = 0x01,
    POSEIDONZ_DIRECT_BLUE       = 0x02
};

enum
{
    POSEIDONZ_SPEED_SLOW        = 0x10,
    POSEIDONZ_SPEED_FAST        = 0x05
};

class PoseidonZRGBController
{
public:
    PoseidonZRGBController(hid_device* dev_handle, const char* path);
    ~PoseidonZRGBController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SetMode(unsigned char mode, unsigned char direction, unsigned char speed);
    void SetLEDsDirect(std::vector<RGBColor> colors);
    void SetLEDs(std::vector<RGBColor> colors);

private:
    hid_device*             dev;
    unsigned char           active_mode;
    unsigned char           active_direction;
    unsigned char           active_speed;
    std::string             location;

    void SendControl
        (
        unsigned char   profile_to_activate,
        unsigned char   profile_to_edit,
        unsigned char   direction,
        unsigned char   mode,
        unsigned char   brightness,
        unsigned char   speed
        );

    void SendColor
        (
        unsigned char   profile_to_edit,
        unsigned char   color_channel,
        unsigned char*  color_data
        );
};

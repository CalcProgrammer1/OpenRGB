/*---------------------------------------------------------*\
| RedSquareKeyroxController.h                               |
|                                                           |
|   Driver for Red Square Keyrox                            |
|                                                           |
|   cafeed28                                    03 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define PACKET_DATA_LENGTH      520

/*-----------------------------*\
|  Red Square Keyrox variants   |
\*-----------------------------*/
enum
{
    KEYROX_VARIANT_TKL,
};

/*---------------------------------------*\
|  Modes                                  |
\*---------------------------------------*/
enum
{
    WAVE_MODE_VALUE                 = 0x00,
    CONST_MODE_VALUE                = 0x01,
    BREATHE_MODE_VALUE              = 0x02,
    HEARTRATE_MODE_VALUE            = 0x03,
    POINT_MODE_VALUE                = 0x04,
    WINNOWER_MODE_VALUE             = 0x05,
    STARS_MODE_VALUE                = 0x06,
    SPECTRUM_MODE_VALUE             = 0x07,
    PLUMFLOWER_MODE_VALUE           = 0x08,
    SHOOT_MODE_VALUE                = 0x09,
    AMBILIGHT_ROTATE_MODE_VALUE     = 0x0A,
    RIPPLE_MODE_VALUE               = 0x0B,
    CUSTOM_MODE_VALUE               = 0x0C,
};

/*-----------------------------*\
|  Other settings               |
\*-----------------------------*/
enum
{
    KEYROX_BRIGHTNESS_MIN = 0x00,
    KEYROX_BRIGHTNESS_MAX = 0x7F,
    KEYROX_SPEED_MIN      = 0x00,
    KEYROX_SPEED_MAX      = 0x04,
};


class RedSquareKeyroxController
{
public:
    RedSquareKeyroxController(hid_device *dev_handle, const hid_device_info &info, int variant, std::string dev_name);
    ~RedSquareKeyroxController();

    int                 GetVariant();
    std::string         GetDeviceLocation();
    std::string         GetNameString();
    std::string         GetSerialString();

    int                 GetDirectionLRUD(int direction); // Direction for Left-Right-Up-Down modes
    int                 GetDirectionUD(int direction); // Direction for Up-Down modes

    void                SetLedSequencePositions(std::vector<unsigned int> positions);
    void                SetMode(std::vector<mode> modes, int active_mode);
    void                SetModeData(std::vector<mode> modes, int active_mode);
    void                SetLEDsData(std::vector<mode> modes, int active_mode, std::vector<RGBColor> colors);

    void                Send(unsigned char data[PACKET_DATA_LENGTH]);

protected:
    hid_device*         dev;

private:
    int                         variant;
    std::string                 location;
    std::string                 name;
    std::string                 serial_number;
    std::vector<unsigned int>   led_sequence_positions;
};

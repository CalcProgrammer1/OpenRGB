/*---------------------------------------------------------*\
| WitmodKeyboardController.h                                 |
|                                                           |
|   Driver for Witmod keyboards                             |
|                                                           |
|   vlack                                       03 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "KeyboardLayoutManager.h"

#define WITMOD_PACKET_DATA_LENGTH       64

/*---------------------------------------------------------*\
| The device info request answers with a string such as     |
|                                                           |
|     2M252,01,KB,JC,GK8120SKRGB,V1.04.04                   |
|                                                           |
| where the fifth field is the board model and the last is  |
| the firmware version.  GK8120 is the full size board and  |
| GK8110 the TKL sized one.  These are Witmod platform       |
| codes; the same boards are resold under several brands.   |
\*---------------------------------------------------------*/
#define WITMOD_INFO_COMMAND             0x0D
#define WITMOD_INFO_RETRIES             10
#define WITMOD_INFO_TIMEOUT             150
#define WITMOD_MODEL_GK8110             "GK8110"
#define WITMOD_MODEL_GK8120             "GK8120"

/*---------------------------------------*\
|  Modes                                  |
\*---------------------------------------*/
enum
{
    WITMOD_CONST_MODE_VALUE                   = 0x00, // static
    WITMOD_BREATHE_MODE_VALUE                 = 0x01, // breath
    WITMOD_WAVE_MODE_VALUE                    = 0x02, // wave
    WITMOD_FADE_MODE_VALUE                    = 0x03, // neon
    WITMOD_RADAR_MODE_VALUE                   = 0x04, // radar
    WITMOD_STAR_MODE_VALUE                    = 0x06, // интерактив
    WITMOD_LINE_MODE_VALUE                    = 0x07, // сияние
    WITMOD_RIPPLE_MODE_VALUE                  = 0x08, // рябь интерактив
    WITMOD_STARS_MODE_VALUE                   = 0x09, // мерцание
    WITMOD_CUSTOM_MODE_VALUE                  = 0x0A,
    WITMOD_CROSS_MODE_VALUE                   = 0x0B, // скрещивание
    WITMOD_WTF_MODE_VALUE                     = 0x0C, // быстрый отклик
    WITMOD_RIPPLE_RANDOM_MODE_VALUE           = 0x0E, // рябь
    WITMOD_RUNNING_LINE_MODE_VALUE            = 0x0F, // бегущая строка
    WITMOD_FIREWORK_MODE_VALUE                = 0x10, // firework
};

/*-----------------------------*\
|  Other settings               |
\*-----------------------------*/
enum
{
    WITMOD_BRIGHTNESS_MIN = 0x00,
    WITMOD_BRIGHTNESS_MAX = 0x04,
    WITMOD_SPEED_MIN      = 0x00,
    WITMOD_SPEED_MAX      = 0x04,
};


class WitmodKeyboardController
{
public:
    WitmodKeyboardController(hid_device *dev_handle, const hid_device_info &info, std::string dev_name);
    ~WitmodKeyboardController();

    std::string         GetDeviceLocation();
    std::string         GetNameString();
    std::string         GetSerialString();
    std::string         GetVendorString();
    std::string         GetVersionString();
    KEYBOARD_SIZE       GetKeyboardSize();

    int                 GetDirection(int direction);
    int                 GetDirectionRound(int direction);

    void                SetMode(mode m);
    void                SetLEDsData(std::vector<RGBColor> colors, std::vector<led> leds);

protected:
    hid_device*         dev;

private:
    void                        ReadDeviceInfo();

    std::string                 location;
    std::string                 name;
    std::string                 serial_number;
    std::string                 vendor;
    std::string                 version;
    KEYBOARD_SIZE               keyboard_size;
    std::vector<unsigned int>   led_sequence_positions;
};

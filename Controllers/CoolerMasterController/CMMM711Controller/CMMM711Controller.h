/*---------------------------------------------------------*\
| CMMM711Controller.h                                       |
|                                                           |
|   Driver for Cooler Master M711 mouse                     |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <array>
#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define CM_MM711_PACKET_SIZE 65
#define CM_MM711_COLOUR_MODE_DATA_SIZE (sizeof(colour_mode_data[0]) / sizeof(colour_mode_data[0][0]))
#define CM_MM711_HEADER_DATA_SIZE (sizeof(argb_header_data) / sizeof(argb_headers) )
#define CM_MM711_INTERRUPT_TIMEOUT 250
#define CM_MM711_DEVICE_NAME_SIZE (sizeof(device_name) / sizeof(device_name[ 0 ]))
#define HID_MAX_STR 255

enum
{
    CM_MM711_REPORT_BYTE            = 1,
    CM_MM711_COMMAND_BYTE           = 2,
    CM_MM711_FUNCTION_BYTE          = 3,
    CM_MM711_ZONE_BYTE              = 4,
    CM_MM711_MODE_BYTE              = 5,
    CM_MM711_SPEED_BYTE             = 6,
    CM_MM711_NFI_1                  = 7,
    CM_MM711_NFI_2                  = 8,
    CM_MM711_NFI_3                  = 9,
    CM_MM711_BRIGHTNESS_BYTE        = 10,
    CM_MM711_RED_BYTE               = 11,
    CM_MM711_GREEN_BYTE             = 12,
    CM_MM711_BLUE_BYTE              = 13,
};

enum
{
    CM_MM711_MODE_STATIC            = 0,    //Static Mode
    CM_MM711_MODE_BREATHING         = 1,    //Breathing Mode
    CM_MM711_MODE_SPECTRUM_CYCLE    = 2,    //Spectrum Cycle Mode
    CM_MM711_MODE_INDICATOR         = 4,    //Indicator Mode
    CM_MM711_MODE_CUSTOM            = 176,  //Custom LED Control
    CM_MM711_MODE_OFF               = 254   //Turn Off
};

enum
{
    CM_MM711_SPEED_SLOWEST          = 0x5F, // Slowest speed
    CM_MM711_SPEED_NORMAL           = 0x38, // Normal speed
    CM_MM711_SPEED_FASTEST          = 0x20, // Fastest speed
};

class CMMM711Controller
{
public:
    CMMM711Controller(hid_device* dev_handle, char *_path);
    ~CMMM711Controller();

    std::string GetDeviceName();
    std::string GetSerial();
    std::string GetLocation();

    uint8_t     GetZoneIndex();
    uint8_t     GetMode();
    uint8_t     GetLedRed();
    uint8_t     GetLedGreen();
    uint8_t     GetLedBlue();
    uint8_t     GetLedSpeed();
    RGBColor    GetWheelColour();
    RGBColor    GetLogoColour();

    void        SendUpdate(uint8_t mode, uint8_t speed, RGBColor colour, uint8_t brightness);
    void        SetLedsDirect(RGBColor wheel_colour, RGBColor logo_colour);
    void        SendSavePacket();
private:
    std::string device_name;
    std::string serial;
    std::string location;
    hid_device* dev;

    uint8_t     current_mode;
    uint8_t     current_speed;

    uint8_t     current_brightness;
    uint8_t     current_red;
    uint8_t     current_green;
    uint8_t     current_blue;
    RGBColor    wheel_colour;
    RGBColor    logo_colour;

    void        GetColourStatus();
    void        GetCustomStatus();
    void        GetModeStatus();
    void        SendInitPacket();
    void        SendApplyPacket(uint8_t mode);
};

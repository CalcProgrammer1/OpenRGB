/*---------------------------------------------------------*\
| CMMMController.h                                          |
|                                                           |
|   Driver for Cooler Master mouse                          |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|   Dracrius                                    12 Mar 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <array>
#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define CM_MM_PACKET_SIZE 65
#define CM_MM_COLOUR_MODE_DATA_SIZE (sizeof(colour_mode_data[0]) / sizeof(colour_mode_data[0][0]))
#define CM_MM_HEADER_DATA_SIZE (sizeof(argb_header_data) / sizeof(argb_headers) )
#define CM_MM_INTERRUPT_TIMEOUT 250
#define CM_MM_DEVICE_NAME_SIZE (sizeof(device_name) / sizeof(device_name[ 0 ]))
#define HID_MAX_STR 255

enum
{
    CM_MM530_PID                 = 0x0065,
    CM_MM531_PID                 = 0x0097,
    CM_MM711_PID                 = 0x0101,
    CM_MM720_PID                 = 0x0141,
    CM_MM730_PID                 = 0x0165,
};

enum
{
    CM_MM_REPORT_BYTE            = 1,
    CM_MM_COMMAND_BYTE           = 2,
    CM_MM_FUNCTION_BYTE          = 3,
    CM_MM_ZONE_BYTE              = 4,
    CM_MM_MODE_BYTE              = 5,
    CM_MM_SPEED_BYTE             = 6,
    CM_MM_NFI_1                  = 7,
    CM_MM_NFI_2                  = 8,
    CM_MM_NFI_3                  = 9,
    CM_MM_BRIGHTNESS_BYTE        = 10,
    CM_MM_RED_BYTE               = 11,
    CM_MM_GREEN_BYTE             = 12,
    CM_MM_BLUE_BYTE              = 13,
    CM_MM_SKY_RED_BYTE           = 14,
    CM_MM_SKY_GREEN_BYTE         = 15,
    CM_MM_SKY_BLUE_BYTE          = 16
};

enum
{
    CM_MM5XX_COMMAND             = 0x2C,
    CM_MM7XX_COMMAND             = 0x2B
};

enum
{
    CM_MM_CUSTOM_APPLY           = 0x30, //Also Used for Multilayer Mode
    CM_MM_APPLY                  = 0x28  //Sent Before Update, Unless using a Zoned Mode then UsingZones Before and Apply After
};

enum
{
    CM_MM_MODE_STATIC            = 0,    //Static Mode
    CM_MM_MODE_BREATHING         = 1,    //Breathing Mode
    CM_MM_MODE_SPECTRUM_CYCLE    = 2,    //Spectrum Cycle Mode
    CM_MM_MODE_STARS             = 3,    //Stars Mode
    CM_MM_MODE_INDICATOR         = 4,    //Indicator Mode
    CM_MM_MODE_CUSTOM            = 176,  //Custom LED Control
    CM_MM_MODE_MULTILAYER        = 224,  //Multilayer Mode, i.e. Effect per Zone.
    CM_MM_MODE_OFF               = 254   //Turn Off
};

enum
{
    CM_MM_SPEED_1               = 0x3C,  // Slowest speed
    CM_MM_SPEED_2               = 0x37,
    CM_MM_SPEED_3               = 0x31,  // Normal speed
    CM_MM_SPEED_4               = 0x2C,
    CM_MM_SPEED_5               = 0x26  // Fastest speed
};

class CMMMController
{
public:
    CMMMController(hid_device* dev_handle, char *_path, uint16_t pid, std::string dev_name);
    ~CMMMController();

    std::string GetDeviceVendor();
    std::string GetSerial();
    std::string GetLocation();
    std::string GetName();

    uint16_t    GetProductID();

    uint8_t     GetZoneIndex();
    uint8_t     GetMode();
    uint8_t     GetLedRed();
    uint8_t     GetLedGreen();
    uint8_t     GetLedBlue();
    uint8_t     GetLedSpeed();
    RGBColor    GetWheelColour();
    RGBColor    GetButtonsColour();
    RGBColor    GetLogoColour();

    void        SendUpdate(uint8_t mode, uint8_t speed, RGBColor colour, uint8_t brightness);
    void        SendUpdate(uint8_t mode, uint8_t speed, RGBColor mode_one, RGBColor mode_two, uint8_t brightness);
    void        SetLedsDirect(RGBColor wheel_colour, RGBColor buttons_colour, RGBColor logo_colour);
    void        SendSavePacket();
private:
    std::string name;
    std::string location;
    hid_device* dev;

    uint16_t    product_id;

    uint8_t     command_code;

    uint8_t     current_mode;
    uint8_t     current_speed;

    uint8_t     current_brightness;
    uint8_t     current_red;
    uint8_t     current_green;
    uint8_t     current_blue;

    uint8_t     buttons_bytes[3];
    uint8_t     logo_bytes[3];
    uint8_t     wheel_bytes[3];

    RGBColor    buttons_colour;
    RGBColor    logo_colour;
    RGBColor    wheel_colour;


    void        GetColourStatus();
    void        GetCustomStatus();
    void        GetModeStatus();
    void        SendInitPacket();
    void        SendUsingZonesPacket(uint8_t mode);
    void        SendApplyPacket(uint8_t mode);
    void        SendMultilayerPacket(uint8_t zones[3]);
};

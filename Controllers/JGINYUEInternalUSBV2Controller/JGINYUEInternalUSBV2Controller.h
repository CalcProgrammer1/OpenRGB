/*---------------------------------------------------------*\
| JGINYUEInternalUSBV2Controller.h                          |
|                                                           |
|   Driver for JGINYUE USB motherboard V2                   |
|                                                           |
|   Tong R (tcr020)                             09 Aug 2023 |
|   Liu ShiMeng(Moon dream stars)               06 Aug 2024 |
|   Dongguan Yonghang Electronic Technology Co., Ltd        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "serial_port.h"
#include "RGBController.h"

#define JGINYUE_MAX_ZONES               2
#define JGINYUE_ADDRESSABLE_MAX_LEDS    100

enum
{
    JGINYUE_USB_V2_MODE_SYNC               = 0x0F,
    JGINYUE_USB_V2_MODE_OFF                = 0x10,
    JGINYUE_USB_V2_MODE_STATIC             = 0x11,
    JGINYUE_USB_V2_MODE_BREATHING          = 0x12,
    JGINYUE_USB_V2_MODE_CYCLING            = 0x14,
    JGINYUE_USB_V2_MODE_RANDOM             = 0x15,
    JGINYUE_USB_V2_MODE_SPRING             = 0x16,
    JGINYUE_USB_V2_MODE_WAVE               = 0x17,
    JGINYUE_USB_V2_MODE_WATER              = 0x19,
    JGINYUE_USB_V2_MODE_RAINBOW            = 0x1A,
    JGINYUE_USB_V2_MODE_MULTICOLOR_WAVE    = 0x1B,
    JGINYUE_USB_V2_MODE_MULTICOLOR_CYCLING = 0x1C,
    JGINYUE_USB_V2_MODE_SUNRISE            = 0x1D,
    JGINYUE_USB_V2_MODE_ROTATE_STAR        = 0x1E,
    JGINYUE_USB_V2_MODE_METEOR             = 0x1F,
    JGINYUE_USB_V2_MODE_DIRECT             = 0x20,
    JGINYUE_USB_V2_MODE_CYCLING_BREATHING  = 0x21,
    JGINYUE_USB_V2_MODE_CYCLING_RAINING    = 0x22,
    JGINYUE_USB_V2_MODE_MULTICOLOR_WATER_2 = 0x23,
    JGINYUE_USB_V2_MODE_MULTICOLOR_WATER_1 = 0x24,
    JGINYUE_USB_V2_MODE_HOURGLASS          = 0x25
};

enum
{
    JGINYUE_USB_SPEED_MAX               = 0xFF,
    JGINYUE_USB_SPEED_MIN               = 0x00,
    JGINYUE_USB_SPEED_DEFAULT           = 0x80
};

enum
{
    JGINYUE_DIRECTION_RIGHT             = 0x00,
    JGINYUE_DIRECTION_LEFT              = 0x01
};

enum
{
    JGINYUE_USB_BRIGHTNESS_MAX          = 0xFF,
    JGINYUE_USB_BRIGHTNESS_MIN          = 0x00,
    JGINYUE_USB_BRIGHTNESS_DEFAULT      = 0xFF
};

enum
{
    JGINYUE_USB_V2_ARGB_STRIP_1         = 0x01,
    JGINYUE_USB_V2_ARGB_STRIP_2         = 0x02,
    JGINYUE_USB_V2_ARGB_FAN_1      = 0x04,
    JGINYUE_USB_V2_ARGB_FAN_2      = 0x08,
    JGINYUE_USB_V2_ARGB_FAN_3      = 0x10,
    JGINYUE_USB_V2_ARGB_FAN_4      = 0x20,
    JGINYUE_USB_V2_ARGB_FAN_5      = 0x40,
};

struct AreaConfigurationV2
{
    unsigned char   Area_ID;
    unsigned char   Max_LED_numbers;
    unsigned char   User_LED_numbers;
    unsigned char   Direction;
    unsigned char   Direct_Mode_control;          /* 0x00 = Disabled, 0x01 = Enabled                              */
    unsigned char   Mode_active;
    unsigned char   Color_num;
    unsigned char   Color_Array[30];
    unsigned char   Brightness;
    unsigned char   Speed;
};

class JGINYUEInternalUSBV2Controller
{
public:
    JGINYUEInternalUSBV2Controller(hid_device* jy_hid_device, const char* path,serial_port* jy_cdc_device);
    ~JGINYUEInternalUSBV2Controller();

    unsigned int        GetZoneCount();
    std::string         GetDeviceLocation();
    std::string         GetDeviceName();
    std::string         GetSerialString();
    std::string         GetDeviceFWVersion();

    void WriteZoneMode
        (
        unsigned char   Area,
        unsigned char   Mode,
        unsigned char   Num_LED,
        std::vector<RGBColor> rgb,
        unsigned char   Speed,
        unsigned char   Brightness,
        unsigned char   Direction
        );

    void DirectLEDControl
        (
        RGBColor*       colors,
        unsigned char   num_LEDs,
        unsigned char   Area
        );

    AreaConfigurationV2 device_config[8];
    //TODO,When the perzone mode is supported, these parameters will be used to download device configuartion from the device
    AreaConfigurationV2 device_config_Global;
    //TODO,Can sync its data to other zones,will be used once the perzone mode is supported
    bool                support_Global_zone;

private:
    void                Init_device();
    void                Init_Zone(int zone);
    unsigned char       ZoneCount;

    hid_device*         jy_hid_interface;
    serial_port*        jy_cdc_interface;
    std::string         location;
    std::string         device_name;
};

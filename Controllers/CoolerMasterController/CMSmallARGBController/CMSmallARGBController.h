/*---------------------------------------------------------*\
| CMSmallARGBController.h                                   |
|                                                           |
|   Driver for Cooler Master Small ARGB controller          |
|                                                           |
|   Chris M (Dr_No)                             31 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <array>
#include <string>
#include <hidapi.h>
#include "RGBController.h"  //Needed to set the direct mode

/*---------------------------------------------------------*\
|  Simple RGB device with 5 modes                           |
\*---------------------------------------------------------*/

#define CM_SMALL_ARGB_PACKET_SIZE       65
#define CM_SMALL_ARGB_INTERRUPT_TIMEOUT 250
#define HID_MAX_STR                     255

enum
{
    CM_SMALL_ARGB_REPORT_BYTE           =  1,
    CM_SMALL_ARGB_COMMAND_BYTE          =  2,
    CM_SMALL_ARGB_FUNCTION_BYTE         =  3,
    CM_SMALL_ARGB_ZONE_BYTE             =  4,
    CM_SMALL_ARGB_MODE_BYTE             =  5,
    CM_SMALL_ARGB_SPEED_BYTE            =  6,
    CM_SMALL_ARGB_COLOUR_INDEX_BYTE     =  7,  //Not used on the small controller
    CM_SMALL_ARGB_BRIGHTNESS_BYTE       =  8,  //0x00 thru 0xFF
    CM_SMALL_ARGB_RED_BYTE              =  9,
    CM_SMALL_ARGB_GREEN_BYTE            = 10,
    CM_SMALL_ARGB_BLUE_BYTE             = 11,
};

struct cm_small_argb_headers
{
    const char*     name;
    unsigned char   header;
    bool            digital;
    unsigned int    count;
};

extern cm_small_argb_headers cm_small_argb_header_data[1];

enum
{
    CM_SMALL_ARGB_MODE_SPECTRUM       = 0x01,   //Spectrum Mode
    CM_SMALL_ARGB_MODE_RELOAD         = 0x02,   //Reload Mode
    CM_SMALL_ARGB_MODE_RECOIL         = 0x03,   //Recoil Mode
    CM_SMALL_ARGB_MODE_BREATHING      = 0x04,   //Breathing Mode
    CM_SMALL_ARGB_MODE_REFILL         = 0x05,   //Refill Mode
    CM_SMALL_ARGB_MODE_DEMO           = 0x06,   //Demo Mode
    CM_SMALL_ARGB_MODE_OFF            = 0x09,   //Turn off
    CM_SMALL_ARGB_MODE_DIRECT         = 0xFE,   //Direct Led Control (possibly N?A for small controller)
    CM_SMALL_ARGB_MODE_PASSTHRU       = 0xFF    //Motherboard Pass Thru Mode
};

enum
{
    CM_SMALL_ARGB_SPEED_SLOWEST       = 0x00,   // Slowest speed
    CM_SMALL_ARGB_SPEED_SLOW          = 0x01,   // Slower speed
    CM_SMALL_ARGB_SPEED_NORMAL        = 0x02,   // Normal speed
    CM_SMALL_ARGB_SPEED_FAST          = 0x03,   // Fast speed
    CM_SMALL_ARGB_SPEED_FASTEST       = 0x04,   // Fastest speed
};

class CMSmallARGBController
{
public:
    CMSmallARGBController(hid_device* dev_handle, char *_path, unsigned char _zone_idx);
    ~CMSmallARGBController();

    std::string         GetDeviceName();
    std::string         GetSerial();
    std::string         GetLocation();

    unsigned char       GetZoneIndex();
    unsigned char       GetMode();
    unsigned char       GetLedRed();
    unsigned char       GetLedGreen();
    unsigned char       GetLedBlue();
    unsigned char       GetLedSpeed();
    bool                GetRandomColours();

    void                SetLedCount(int zone, int led_count);
    void                SetMode(unsigned char mode, unsigned char speed, unsigned char brightness, RGBColor colour, bool random_colours);
    void                SetLedsDirect(RGBColor * led_colours, unsigned int led_count);
private:
    std::string         device_name;
    std::string         location;
    hid_device*         dev;

    unsigned char       zone_index;
    unsigned char       current_mode;
    unsigned char       current_speed;

    unsigned char       current_red;
    unsigned char       current_green;
    unsigned char       current_blue;
    unsigned char       current_brightness;
    bool                bool_random;

    unsigned int        GetLargestColour(unsigned int red, unsigned int green, unsigned int blue);
    unsigned char       GetColourIndex(unsigned char red, unsigned char green, unsigned char blue);
    void                GetStatus();
    void                SendUpdate();
};

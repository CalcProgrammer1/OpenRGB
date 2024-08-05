/*---------------------------------------------------------*\
| CMARGBController.h                                        |
|                                                           |
|   Driver for Cooler Master ARGB controller                |
|                                                           |
|   Chris M (Dr_No)                             10 Oct 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <cstring>
#include <array>
#include <memory>
#include <hidapi.h>
#include "RGBController.h"  //Needed to set the direct mode

#define CM_ARGB_COLOUR_MODE_DATA_SIZE (sizeof(colour_mode_data[0]) / sizeof(colour_mode_data[0][0]))
#define CM_ARGB_HEADER_DATA_SIZE (sizeof(argb_header_data) / sizeof(argb_headers) )
#define CM_ARGB_INTERRUPT_TIMEOUT 250
#define CM_ARGB_PACKET_SIZE 65
#define CM_ARGB_DEVICE_NAME_SIZE (sizeof(device_name) / sizeof(device_name[ 0 ]))
#define CM_RGB_OFFSET -2
#define HID_MAX_STR 255

#define CM_ARGB_BRIGHTNESS_MAX  255
#define CM_ARGB_FW0023          std::string("A202011171238")
#define CM_ARGB_FW0028          std::string("A202105291658")

enum
{
    CM_ARGB_REPORT_BYTE         = 1,
    CM_ARGB_COMMAND_BYTE        = 2,
    CM_ARGB_FUNCTION_BYTE       = 3,
    CM_ARGB_ZONE_BYTE           = 4,
    CM_ARGB_MODE_BYTE           = 5,
    CM_ARGB_COLOUR_INDEX_BYTE	= 6,
    CM_ARGB_SPEED_BYTE          = 7,
    CM_ARGB_BRIGHTNESS_BYTE     = 8,
    CM_ARGB_RED_BYTE            = 9,
    CM_ARGB_GREEN_BYTE          = 10,
    CM_ARGB_BLUE_BYTE           = 11
};

struct argb_headers
{
    const char*     name;
    unsigned char   header;
    bool            digital;
    unsigned int    count;
};

static argb_headers argb_header_data[] =
{
    { "RGB Header",         0xFE, false,  1 },
    { "Digital ARGB1",      0x01, true,  12 },
    { "Digital ARGB2",      0x02, true,  12 },
    { "Digital ARGB3",      0x04, true,  12 },
    { "Digital ARGB4",      0x08, true,  12 },
    //{ "All Digital ARGB",   0xFF, true,  12 }
};

enum
{
    CM_RGB_MODE_MIRAGE          = 0x01, //Mirage
    CM_RGB_MODE_FLASH           = 0x02, //Flash
    CM_RGB_MODE_BREATHING       = 0x03, //Breathing
    CM_RGB_MODE_STATIC          = 0x05, //Static
    CM_RGB_MODE_OFF             = 0x06, //Turn off
    CM_RGB_MODE_PASSTHRU        = 0xFF  //Motherboard Pass Thru Mode
};

enum
{
    CM_ARGB_MODE_OFF            = 0x0B, //Turn off
    CM_ARGB_MODE_SPECTRUM       = 0x01, //Spectrum Mode
    CM_ARGB_MODE_RELOAD         = 0x02, //Reload Mode
    CM_ARGB_MODE_RECOIL         = 0x03, //Recoil Mode
    CM_ARGB_MODE_BREATHING      = 0x04, //Breathing Mode
    CM_ARGB_MODE_REFILL         = 0x05, //Refill Mode
    CM_ARGB_MODE_DEMO           = 0x06, //Demo Mode
    CM_ARGB_MODE_FILLFLOW       = 0x08, //Fill Flow Mode
    CM_ARGB_MODE_RAINBOW        = 0x09, //Rainbow Mode
    CM_ARGB_MODE_STATIC         = 0x0A, //Static Mode
    CM_ARGB_MODE_DIRECT         = 0xFE, //Direct Led Control
    CM_ARGB_MODE_PASSTHRU       = 0xFF  //Motherboard Pass Thru Mode
};

enum
{
    CM_ARGB_SPEED_SLOWEST       = 0x00, // Slowest speed
    CM_ARGB_SPEED_SLOW          = 0x01, // Slower speed
    CM_ARGB_SPEED_NORMAL        = 0x02, // Normal speed
    CM_ARGB_SPEED_FAST          = 0x03, // Fast speed
    CM_ARGB_SPEED_FASTEST       = 0x04, // Fastest speed
};

class CMARGBController
{
public:
    CMARGBController(hid_device* dev_handle, char *_path, unsigned char _zone_idx, std::shared_ptr<std::mutex> cm_mutex);
    ~CMARGBController();

    std::string GetDeviceName();
    std::string GetSerial();
    std::string GetLocation();

    unsigned char GetZoneIndex();
    unsigned char GetMode();
    unsigned char GetLedRed();
    unsigned char GetLedGreen();
    unsigned char GetLedBlue();
    unsigned char GetLedSpeed();
    bool GetRandomColours();
    void SetLedCount(int zone, int led_count);
    void SetMode(uint8_t mode, uint8_t speed, uint8_t brightness, RGBColor colour, bool random_colours);
    void SetLedsDirect(RGBColor * led_colours, unsigned int led_count);

private:
    std::string                 device_name;
    std::string                 location;
    hid_device*                 dev;
    std::shared_ptr<std::mutex> mutex_ptr;

    unsigned char               zone_index;
    unsigned char               current_mode;
    unsigned char               current_speed;

    unsigned char               current_red;
    unsigned char               current_green;
    unsigned char               current_blue;
    unsigned char               current_brightness;
    bool                        bool_random;

    void GetStatus();
    void SendUpdate();
};

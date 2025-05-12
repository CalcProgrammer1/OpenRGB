/*---------------------------------------------------------*\
| GigabyteRGBFusion2USBController.h                         |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion 2 USB motherboard  |
|                                                           |
|   jackun                                      08 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <map>
#include <thread>
#include <hidapi.h>
#include "RGBController.h"

#define GB_CALIBRATION_SIZE (sizeof(GB_Calibrations) / sizeof(GB_Calibrations[0]))

/*-------------------------------------------------------------*\
| Standardising LED naming for external config layout           |
\*-------------------------------------------------------------*/
const uint8_t LED1          = 0x20;
const uint8_t LED2          = 0x21;
const uint8_t LED3          = 0x22;
const uint8_t LED4          = 0x23;
const uint8_t LED5          = 0x24;
const uint8_t LED6          = 0x25;
const uint8_t LED7          = 0x26;
const uint8_t LED8          = 0x27;

/*-------------------------------------------------------------*\
| LED "headers" 0x20..0x27, As seen on Gigabyte X570 Elite board|
| Internal legacy shorthand naming and possibly deprecated      |
\*-------------------------------------------------------------*/
const uint8_t HDR_BACK_IO   = LED1;
const uint8_t HDR_CPU       = LED2;
const uint8_t HDR_LED_2     = LED3;
const uint8_t HDR_PCIE      = LED4;
const uint8_t HDR_LED_C1C2  = LED5;
const uint8_t HDR_D_LED1    = LED6;
const uint8_t HDR_D_LED2    = LED7;
const uint8_t HDR_LED_7     = LED8;

/*-------------------------------------------------------------*\
| IT8297/IT5702 ARGB Headers                                    |
\*-------------------------------------------------------------*/
const uint8_t HDR_D_LED1_RGB = 0x58;
const uint8_t HDR_D_LED2_RGB = 0x59;
/*-------------------------------------------------------------*\
| 0x62 & 0x90-92 found on the new IT5711 controller chip        |
\*-------------------------------------------------------------*/
const uint8_t LED9          = 0x90;
const uint8_t LED10         = 0x91;
const uint8_t LED11         = 0x92;
const uint8_t HDR_D_LED3_RGB = 0x62;
/*------------------------------------------*\
|Defines new mapping for third argb header   |
\*------------------------------------------*/
const uint8_t HDR_D_LED3    = LED8;

enum EffectType
{
    EFFECT_NONE             = 0,
    EFFECT_STATIC           = 1,
    EFFECT_PULSE            = 2,
    EFFECT_BLINKING         = 3,
    EFFECT_COLORCYCLE       = 4,
    // to be continued...
};

enum LEDCount
{
    LEDS_32                 = 0,
    LEDS_64,
    LEDS_256,
    LEDS_512,
    LEDS_1024,
};

struct LEDs
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#pragma pack(push, 1)

struct RGBA
{
    union
    {
        uint8_t raw[4];
        struct
        {
            uint8_t blue;
            uint8_t green;
            uint8_t red;
            uint8_t alpha;
        };
    };
};

typedef std::map< std::string, RGBA >       RGBCalibration;
typedef std::map< std::string, std::string> calibration;

union PktRGB
{
    unsigned char buffer[64];
    struct RGBData
    {
        uint8_t     report_id;
        uint8_t     header;
        uint16_t    boffset;        // In bytes, absolute
        uint8_t     bcount;
        LEDs        leds[19];
        uint16_t    padding0;
    } s;

    PktRGB() : s {}
    {
    }

    void Init(uint8_t header, uint8_t report_id)
    {
        s.report_id = report_id;
        s.header    = header;
        s.boffset   = 0;
        s.bcount    = 0;
        memset(s.leds, 0, sizeof(s.leds));
    }
};

union PktEffect
{
    unsigned char buffer[64];
    struct Effect
    {
        uint8_t report_id;
        uint8_t header;
        uint32_t zone0;             // RGB Fusion seems to set it to pow(2, header - 0x20)
        uint32_t zone1;
        uint8_t reserved0;
        uint8_t effect_type;
        uint8_t max_brightness;
        uint8_t min_brightness;
        uint32_t color0;
        uint32_t color1;
        uint16_t period0;           // Fade in
        uint16_t period1;           // Fade out
        uint16_t period2;           // Hold
        uint16_t period3;
        uint8_t effect_param0;
        uint8_t effect_param1;
        uint8_t effect_param2;
        uint8_t effect_param3;
        uint8_t padding0[30];
    } e;

    PktEffect() : e {}
    {
    }

    void Init(int header, uint8_t report_id)
    {
        memset(buffer, 0, sizeof(buffer));

        e.report_id         = report_id;

        if(header < 8)
        {
            e.header        = 32 + header;  // Set as default
        }
        else
        {
            e.header        = header;
        }

        e.zone0             = (uint32_t)(1 << (e.header - 32));
        e.effect_type       = EFFECT_STATIC;
        e.max_brightness    = 255;
        e.min_brightness    = 0;
        e.color0            = 0x00FF2100;   //orange
        e.period0           = 0;            //Rising Timer - Needs to be 0 for "Direct"
        e.period1           = 1200;
        e.period2           = 200;
        e.period3           = 200;
        e.effect_param0     = 0;            // ex color count to cycle through (max seems to be 7)
        e.effect_param1     = 0;
        e.effect_param2     = 1;            // ex flash repeat count
        e.effect_param3     = 0;
    }
};

/*--------------------------------------------------------------------------------------*\
| Definitions for Initial controller response struct.                                    |
| curr_led_count_high and curr_led_count_low contain the numbers of leds in each header. |
| Byte orders are little endian little-endian 0x00RRGGBB?                                |
| byteorder0 is location of "Spare" calibration location.                                |
| byteorder1 is location of "D_LED1" calibration location.                               |
| byteorder2 is location of "D_LED2/D_LED3" calibration location.                        |
| byteorder3 is location of "Mainboard" calibration location.                            |
| byteorder4 is location of fifth calibration location.                                  |
| *Note that the calibration locations aren't fully understood yet for the IT5711.       |
\*--------------------------------------------------------------------------------------*/
struct IT8297Report
{
    uint8_t report_id;
    uint8_t product;
    uint8_t device_num;
    uint8_t total_leds;
    uint32_t fw_ver;
    uint8_t curr_led_count_high;
    uint8_t curr_led_count_low;
    uint16_t reserved0;
    char str_product[28];
    uint32_t byteorder0;
    uint32_t byteorder1;
    uint32_t byteorder2;
    uint32_t byteorder3;
    uint32_t chip_id;
    uint32_t byteorder4;
};

#pragma pack(pop)

class RGBFusion2USBController
{
public:
    RGBFusion2USBController(hid_device* handle, const char *path, std::string mb_name, uint16_t pid);
    ~RGBFusion2USBController();

    void            SetStripColors
                        (
                        unsigned int    hdr,
                        RGBColor *      colors,
                        unsigned int    num_colors,
                        int             single_led      = -1
                        );

    void            ResetController(uint16_t pid);
    uint16_t        GetProductID() const;
    void            SetLEDEffect(unsigned int led, int mode, unsigned int speed, unsigned char brightness, bool random, unsigned char red, unsigned char green, unsigned char blue);
    void            SetLedCount(unsigned int led, unsigned int count);
    void            SetMode(int mode);
    bool            ApplyEffect();
    bool            DisableBuiltinEffect(int enable_bit, int mask);
    void            SetCalibration();
    std::string     GetDeviceName();
    std::string     GetDeviceDescription();
    std::string     GetDeviceLocation();
    std::string     GetFWVersion();
    std::string     GetSerial();

private:
    bool            EnableBeat(bool enable);
    bool            SendPacket(uint8_t a, uint8_t b, uint8_t c = 0);
    int             SendPacket(unsigned char* packet);
    RGBA            GetCalibration( std::string rgb_order);
    void            SetCalibrationBuffer(std::string rgb_order, uint8_t* buffer, uint8_t offset);

    hid_device*             dev;
    uint16_t                product_id;
    int                     mode;
    IT8297Report            report;
    std::string             name;
    std::string             description;
    std::string             location;
    std::string             version;
    std::string             chip_id;
    int                     effect_disabled = 0;
    int                     report_id = 0xCC;
    LEDCount                D_LED1_count;
    LEDCount                D_LED2_count;
    LEDCount                D_LED3_count;   //Third ARGB header count
};
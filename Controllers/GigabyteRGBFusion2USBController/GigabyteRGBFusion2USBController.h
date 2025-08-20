/*---------------------------------------------------------*\
| GigabyteRGBFusion2USBController.h                         |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion 2 USB motherboard  |
|                                                           |
|   jackun                                      08 Jan 2020 |
|   megadjc                                     31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <algorithm>
#include <cstring>
#include <hidapi.h>
#include <map>
#include "RGBController.h"

#define FUSION2_USB_BUFFER_SIZE   64

/*--------------------------------------------------------*\
| Base LED mappings found on all controllers.              |
\*--------------------------------------------------------*/
const uint8_t LED1              = 0;
const uint8_t LED2              = 1;
const uint8_t LED3              = 2;
const uint8_t LED4              = 3;
const uint8_t LED5              = 4;
const uint8_t LED6              = 5;
const uint8_t LED7              = 6;
const uint8_t LED8              = 7;

/*--------------------------------------------------------*\
| IT8297/IT5701/IT5702 ARGB Headers                        |
\*--------------------------------------------------------*/
const uint8_t HDR_D_LED1        = LED6;
const uint8_t HDR_D_LED2        = LED7;
const uint8_t HDR_D_LED1_RGB    = 0x58;
const uint8_t HDR_D_LED2_RGB    = 0x59;

/*--------------------------------------------------------*\
| Additional LED mappings found on IT5711 controllers.     |
\*--------------------------------------------------------*/
const uint8_t LED9              = 8;
const uint8_t LED10             = 9;
const uint8_t LED11             = 10;

/*--------------------------------------------------------*\
| IT5711 additional ARGB Headers.                          |
\*--------------------------------------------------------*/
const uint8_t HDR_D_LED3        = LED8;
const uint8_t HDR_D_LED4        = LED9;
const uint8_t HDR_D_LED3_RGB    = 0x62;
const uint8_t HDR_D_LED4_RGB    = 0x63;

/*---------------------------------------------------------*\
| Effects mode list                                         |
\*---------------------------------------------------------*/
enum EffectType
{
    EFFECT_NONE                 = 0,
    EFFECT_STATIC               = 1,
    EFFECT_PULSE                = 2,
    EFFECT_BLINKING             = 3,
    EFFECT_COLORCYCLE           = 4,
    EFFECT_WAVE                 = 6,
    EFFECT_RANDOM               = 8,
    EFFECT_WAVE1                = 9,
    EFFECT_WAVE2                = 10,
    EFFECT_WAVE3                = 11,
    EFFECT_WAVE4                = 12,
    EFFECT_DFLASH               = 15,
    // to be continued...
};

/*---------------------------------------------------------*\
| Low level strip length divisions                          |
\*---------------------------------------------------------*/
enum LEDCount
{
    LEDS_32                     = 0,
    LEDS_64,
    LEDS_256,
    LEDS_512,
    LEDS_1024,
};

/*---------------------------------------------------------*\
| Defines the RGB led data structure.                       |
\*---------------------------------------------------------*/
struct LEDs
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

/*---------------------------------------------------------*\
| Defines structure for low level calibration data.         |
\*---------------------------------------------------------*/
struct CalibrationData
{
    uint32_t dled[4]   = {0};
    uint32_t spare[4]  = {0};
    uint32_t mainboard = 0;
};

/*---------------------------------------------------------*\
| Defines structure for high level calibration data.        |
\*---------------------------------------------------------*/
struct EncodedCalibration
{
    std::string dled[4];
    std::string spare[4];
    std::string mainboard;
};

#pragma pack(push, 1)

/*---------------------------------------------------------*\
| Packet structure for applying effects                     |
\*---------------------------------------------------------*/
union PktEffectApply
{
    unsigned char buffer[FUSION2_USB_BUFFER_SIZE];
    struct apply_data
    {
        uint8_t  report_id  = 0xCC;
        uint8_t  command_id = 0x28;
        uint32_t zone_sel0  = 0;
        uint32_t zone_sel1  = 0;
        uint8_t  padding[54];
    } a;

    PktEffectApply() : a {}
    {
        std::memset(a.padding, 0, sizeof(a.padding));
    }
};

/*---------------------------------------------------------*\
| Single LED Calibration struct                             |
\*---------------------------------------------------------*/
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

/*---------------------------------------------------------*\
| Packet structure for ARGB headers (addressable)           |
\*---------------------------------------------------------*/
union PktRGB
{
    unsigned char buffer[FUSION2_USB_BUFFER_SIZE];
    struct RGBData
    {
        uint8_t     report_id;
        uint8_t     header;
        uint16_t    boffset;
        uint8_t     bcount;
        LEDs        leds[19];
        uint16_t    padding0;
    } s;

    PktRGB() : s {}
    {
    }

    void Init(uint8_t header, uint8_t report_id)
    {
        switch(header)
        {
            case LED4:
            case HDR_D_LED2:
                header = HDR_D_LED2_RGB;
                break;
            case HDR_D_LED3:
                header = HDR_D_LED3_RGB;
                break;
            case HDR_D_LED4:
                header = HDR_D_LED4_RGB;
                break;
            default:
                header = HDR_D_LED1_RGB;
                break;
        }
        s.report_id = report_id;
        s.header    = header;
        s.boffset   = 0;
        s.bcount    = 0;
        memset(s.leds, 0, sizeof(s.leds));
    }
};

/*---------------------------------------------------------*\
| Packet structure for hardware effects                     |
| Default values for Hardware Effects mode.                 |
| Old init values.                                          |
| (All values 0 unless otherwise noted below)               |
| e.color0        = 0x00FF2100;    //orange                 |
| e.period1       = 1200;                                   |
| e.period2       = 200;                                    |
| e.period3       = 200;                                    |
| e.effect_param2 = 1;                                      |
\*---------------------------------------------------------*/
union PktEffect
{
    unsigned char buffer[FUSION2_USB_BUFFER_SIZE];
    struct Effect
    {
        uint8_t report_id       = 0;
        uint8_t header          = 0;
        uint32_t zone0          = 0;    // RGB Fusion sets it to pow(2, led)
        uint32_t zone1          = 0;
        uint8_t reserved0       = 0;
        uint8_t effect_type     = EFFECT_STATIC;
        uint8_t max_brightness  = 255;
        uint8_t min_brightness  = 0;
        uint32_t color0         = 0;
        uint32_t color1         = 0;
        uint16_t period0        = 0;    // Fade in - Rising Timer - Needs to be 0 for "Direct"
        uint16_t period1        = 0;    // Fade out
        uint16_t period2        = 0;    // Hold
        uint16_t period3        = 0;
        uint8_t effect_param0   = 0;    // ex color count to cycle through (max seems to be 7)
        uint8_t effect_param1   = 0;
        uint8_t effect_param2   = 0;    // ex flash repeat count
        uint8_t effect_param3   = 0;
        uint8_t padding0[30];
    } e;

    PktEffect() : e {}
    {
    }

    void Init(int led, uint8_t report_id, uint16_t pid)
    {
        memset(e.padding0, 0, sizeof(e.padding0));

        e.report_id         = report_id;
        if(led == -1)
        {
            e.zone0  = (pid == 0x5711) ? 0x07FF : 0xFF;
            e.header = 0x20;
        }
        else if(led < 8)
        {
            e.zone0  = 1U << led;
            e.header = 0x20 + led;
        }
        else if(led < 11)
        {
            e.zone0  = 1U << led;
            e.header = 0x90 + (led - 8);
        }
        else
        {
            e.zone0  = 0;
            e.header = 0;
        }
    }
};

/*---------------------------------------------------------*\
| Basic Controller Init Struct                              |
\*---------------------------------------------------------*/
struct IT8297Report
{
    uint8_t  report_id;
    uint8_t  product;
    uint8_t  device_num;
    uint8_t  strip_detect;
    uint32_t fw_ver;
    uint8_t  curr_led_count_high;
    uint8_t  curr_led_count_low;
    uint8_t  strip_ctrl_length1;
    uint8_t  support_cmd_flag;
    char     str_product[28];
    uint32_t cal_spare0;
    uint32_t cal_strip0;
    uint32_t cal_strip1;
    uint32_t rgb_cali;
    uint32_t chip_id;
    uint32_t cal_spare1;
};

/*---------------------------------------------------------*\
| CC61 Calibration Struct (For IT5711)                      |
\*---------------------------------------------------------*/
struct IT5711Calibration
{
    uint8_t  report_id;
    uint8_t  reserved[3];
    uint32_t cal_strip2;
    uint32_t cal_strip3;
    uint32_t cal_spare2;
    uint32_t cal_spare3;
    uint8_t  padding[44];
};

/*---------------------------------------------------------*\
| CC33 Set Calibration Struct                               |
\*---------------------------------------------------------*/
struct CMD_0x33
{
    uint8_t  report_id;
    uint8_t  command_id;
    uint32_t d_strip_c0;
    uint32_t d_strip_c1;
    uint32_t rgb_cali;
    uint32_t c_spare0;
    uint32_t c_spare1;
    uint32_t d_strip_c2;
    uint32_t d_strip_c3;
    uint32_t c_spare2;
    uint32_t c_spare3;
    uint8_t  reserved[25];
};

#pragma pack(pop)

class RGBFusion2USBController
{
public:
    RGBFusion2USBController(hid_device* handle, const char *path, std::string mb_name, uint16_t pid);
    ~RGBFusion2USBController();

    bool                    RefreshHardwareInfo();
    void                    ResetController();
    uint16_t                GetProductID();
    uint8_t                 GetDeviceNum();
    void                    SetStripColors(unsigned int hdr, RGBColor * colors, unsigned int num_colors, int single_led = -1);
    void                    SetLEDEffect(int led, int mode, unsigned int speed, unsigned char brightness, bool random, uint32_t* color);
    void                    SetLedCount(unsigned int c0, unsigned int c1, unsigned int c2, unsigned int c3);
    void                    SetMode(int mode);
    bool                    ApplyEffect(bool batch_commit = false);
    bool                    SetStripBuiltinEffectState(int hdr, bool enable);
    EncodedCalibration      GetCalibration(bool refresh_from_hw = false);
    bool                    SetCalibration(const EncodedCalibration& cal, bool refresh_from_hw);
    std::string             GetDeviceName();
    std::string             GetDeviceDescription();
    std::string             GetDeviceLocation();
    std::string             GetFWVersion();
    std::string             GetSerial();

private:
    bool                    SaveLEDState(bool enable);
    bool                    SaveCalState();
    bool                    EnableLampArray(bool enable);
    bool                    EnableBeat(bool enable);
    bool                    SendPacket(uint8_t a, uint8_t b, uint8_t c = 0);
    int                     SendPacket(unsigned char* packet);
    uint32_t                EncodeCalibrationBuffer(const std::string& rgb_order);
    std::string             DecodeCalibrationBuffer(uint32_t value) const;
    hid_device*             dev;
    int                     device_num;
    uint16_t                product_id;
    uint32_t                effect_zone_mask = 0;
    int                     mode;
    IT8297Report            report;
    IT5711Calibration       cali;
    CalibrationData         cal_data;
    std::string             name;
    std::string             description;
    std::string             location;
    std::string             version;
    std::string             chip_id;
    int                     effect_disabled = 0;
    int                     report_id = 0xCC;
    bool                    report_loaded = false;
    bool                    cali_loaded   = false;
    LEDCount                new_d1;
    LEDCount                new_d2;
    LEDCount                new_d3;
    LEDCount                new_d4;
    LEDCount                D_LED1_count;
    LEDCount                D_LED2_count;
    LEDCount                D_LED3_count;
    LEDCount                D_LED4_count;
};

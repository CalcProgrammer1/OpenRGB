/*-----------------------------------------*\
|  GigabyteRGBFusion2USBController.h        |
|                                           |
|  Definitions and types for Gigabyte Aorus |
|  RGB Fusion 2.0 USB lighting controller   |
|                                           |
|  jackun 1/8/2020                          |
\*-----------------------------------------*/

#include "RGBController.h"
#include <cstring>
#include <hidapi/hidapi.h>

#pragma once

// LED "headers" 0x20..0x27, As seen on Gigabyte X570 Elite board
const uint8_t HDR_BACK_IO  = 0x20;
const uint8_t HDR_CPU      = 0x21;
const uint8_t HDR_LED_2    = 0x22;
const uint8_t HDR_PCIE     = 0x23;
const uint8_t HDR_LED_C1C2 = 0x24;
const uint8_t HDR_D_LED1   = 0x25;
const uint8_t HDR_D_LED2   = 0x26;
const uint8_t HDR_LED_7    = 0x27;

const uint8_t HDR_D_LED1_RGB = 0x58; // FIXME assuming that it is 0x58 for all boards
const uint8_t HDR_D_LED2_RGB = 0x59;

enum EffectType
{
    EFFECT_NONE = 0,
    EFFECT_STATIC = 1,
    EFFECT_PULSE = 2,
    EFFECT_BLINKING = 3,
    EFFECT_COLORCYCLE = 4,
    // to be continued...
};

enum LEDCount
{
    LEDS_32 = 0,
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

union PktRGB
{
    unsigned char buffer[64];
    struct RGBData
    {
        uint8_t     report_id;
        uint8_t     header;
        uint16_t    boffset; // in bytes, absolute
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
        uint32_t zone0; // rgb fusion seems to set it to pow(2, header - 0x20)
        uint32_t zone1;
        uint8_t reserved0;
        uint8_t effect_type;
        uint8_t max_brightness;
        uint8_t min_brightness;
        uint32_t color0;
        uint32_t color1;
        uint16_t period0; // fade in
        uint16_t period1; // fade out
        uint16_t period2; // hold
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
        e.report_id = report_id;
        if (header < 8)
            e.header = 32 + header; // set as default
        else
            e.header = header;
        e.zone0 = (uint32_t)(1 << (e.header - 32));
        e.effect_type = EFFECT_STATIC;
        e.max_brightness = 100;
        e.min_brightness = 0;
        e.color0 = 0x00FF2100; //orange
        e.period0 = 1200;
        e.period1 = 1200;
        e.period2 = 200;
        e.period3 = 200;
        e.effect_param0 = 0; // ex color count to cycle through (max seems to be 7)
        e.effect_param1 = 0;
        e.effect_param2 = 1; // ex flash repeat count
        e.effect_param3 = 0;
    }
};

struct IT8297Report
{
    uint8_t report_id;
    uint8_t product;
    uint8_t device_num;
    uint8_t total_leds;
    uint32_t fw_ver;
    uint16_t curr_led_count;
    uint16_t reserved0;
    char str_product[32]; // might be 28 and an extra byteorder3
    uint32_t byteorder0; // is little-endian 0x00RRGGBB ?
    uint32_t byteorder1;
    uint32_t byteorder2;
    uint32_t chip_id;
    uint32_t reserved1;
};

#pragma pack(pop)

class RGBFusion2USBController
{
public:
    RGBFusion2USBController(hid_device* handle, const char *path, std::string mb_name);
    ~RGBFusion2USBController();

    void            SetStripColors
                        (
                        unsigned int    hdr,
                        RGBColor *      colors,
                        unsigned int    num_colors,
                        int             single_led      = -1
                        );

    void            SetLEDEffect(unsigned int led, int mode, unsigned int speed, bool random, unsigned char red, unsigned char green, unsigned char blue);
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

    hid_device*             dev;
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
};

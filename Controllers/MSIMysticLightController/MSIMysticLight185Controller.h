/*-----------------------------------------*\
|  MSIMysticLight185Controller.h            |
|                                           |
|  Definitions and types for MSI Mystic     |
|   Light (185-byte) USB lighting           |
|   controllers                             |
|                                           |
|  T-bond 3/4/2020                          |
|  Adam Honse 3/6/2021                      |
\*-----------------------------------------*/

#include "RGBController.h"
#include <cstring>
#include <hidapi/hidapi.h>
#include <limits>

#pragma once

enum MSI_ZONE
{
    MSI_ZONE_J_RGB_1                = 1,
    MSI_ZONE_J_RGB_2                = 2,
    MSI_ZONE_J_PIPE_1               = 3,
    MSI_ZONE_J_PIPE_2               = 4,
    MSI_ZONE_J_RAINBOW_1            = 5,
    MSI_ZONE_J_RAINBOW_2            = 6,
    MSI_ZONE_J_CORSAIR              = 7,
    MSI_ZONE_J_CORSAIR_OUTERLL120   = 8,
    MSI_ZONE_ON_BOARD_LED           = 9,
    MSI_ZONE_ON_BOARD_LED_1         = 10,
    MSI_ZONE_ON_BOARD_LED_2         = 11,
    MSI_ZONE_ON_BOARD_LED_3         = 12,
    MSI_ZONE_ON_BOARD_LED_4         = 13,
    MSI_ZONE_ON_BOARD_LED_5         = 14,
    MSI_ZONE_ON_BOARD_LED_6         = 15,
    MSI_ZONE_ON_BOARD_LED_7         = 16,
    MSI_ZONE_ON_BOARD_LED_8         = 17,
    MSI_ZONE_ON_BOARD_LED_9         = 18
};

struct ZoneDescription
{
    std::string name;
    MSI_ZONE    value;
};

enum MSI_MODE
{
    MSI_MODE_DISABLE                    = 0,
    MSI_MODE_STATIC                     = 1,
    MSI_MODE_BREATHING                  = 2,
    MSI_MODE_FLASHING                   = 3,
    MSI_MODE_DOUBLE_FLASHING            = 4,
    MSI_MODE_LIGHTNING                  = 5,
    MSI_MODE_MSI_MARQUEE                = 6,
    MSI_MODE_METEOR                     = 7,
    MSI_MODE_WATER_DROP                 = 8,
    MSI_MODE_MSI_RAINBOW                = 9,
    MSI_MODE_POP                        = 10,
    MSI_MODE_RAP                        = 11,
    MSI_MODE_JAZZ                       = 12,
    MSI_MODE_PLAY                       = 13,
    MSI_MODE_MOVIE                      = 14,
    MSI_MODE_COLOR_RING                 = 15,
    MSI_MODE_PLANETARY                  = 16,
    MSI_MODE_DOUBLE_METEOR              = 17,
    MSI_MODE_ENERGY                     = 18,
    MSI_MODE_BLINK                      = 19,
    MSI_MODE_CLOCK                      = 20,
    MSI_MODE_COLOR_PULSE                = 21,
    MSI_MODE_COLOR_SHIFT                = 22,
    MSI_MODE_COLOR_WAVE                 = 23,
    MSI_MODE_MARQUEE                    = 24,
    MSI_MODE_RAINBOW                    = 25,
    MSI_MODE_RAINBOW_WAVE               = 26,
    MSI_MODE_VISOR                      = 27,
    MSI_MODE_JRAINBOW                   = 28,
    MSI_MODE_RAINBOW_FLASHING           = 29,
    MSI_MODE_RAINBOW_DOUBLE_FLASHING    = 30,
    MSI_MODE_RANDOM                     = 31,
    MSI_MODE_FAN_CONTROL                = 32,
    MSI_MODE_DISABLE_2                  = 33,
    MSI_MODE_COLOR_RING_FLASHING        = 34,
    MSI_MODE_COLOR_RING_DOUBLE_FLASHING = 35,
    MSI_MODE_STACK                      = 36,
    MSI_MODE_CORSAIR_QUE                = 37,
    MSI_MODE_FIRE                       = 38,
    MSI_MODE_LAVA                       = 39,
};

enum MSI_SPEED
{
    MSI_SPEED_LOW                       = 0,
    MSI_SPEED_MEDIUM                    = 1,
    MSI_SPEED_HIGH                      = 2,
};

enum MSI_FAN_TYPE
{
    MSI_FAN_TYPE_SP                     = 0,
    MSI_FAN_TYPE_HD                     = 1,
    MSI_FAN_TYPE_LL                     = 2,
};

enum MSI_BRIGHTNESS
{
    MSI_BRIGHTNESS_OFF                  = 0,
    MSI_BRIGHTNESS_LEVEL_10             = 1,
    MSI_BRIGHTNESS_LEVEL_20             = 2,
    MSI_BRIGHTNESS_LEVEL_30             = 3,
    MSI_BRIGHTNESS_LEVEL_40             = 4,
    MSI_BRIGHTNESS_LEVEL_50             = 5,
    MSI_BRIGHTNESS_LEVEL_60             = 6,
    MSI_BRIGHTNESS_LEVEL_70             = 7,
    MSI_BRIGHTNESS_LEVEL_80             = 8,
    MSI_BRIGHTNESS_LEVEL_90             = 9,
    MSI_BRIGHTNESS_LEVEL_100            = 10, 
};

struct Color
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

struct CorsairZoneData
{
    unsigned char effect                    = MSI_MODE_STATIC;
    Color         color                     { 0, 0, 0 };
    unsigned char fan_flags                 = 40;
    unsigned char corsair_quantity;
    unsigned char padding[4];
    unsigned char is_individual             = 0;
};

struct ZoneData
{
    unsigned char effect                    = MSI_MODE_STATIC;
    Color         color                     { 0, 0, 0 };
    unsigned char speedAndBrightnessFlags   = 0;
    Color         color2                    { 0, 0, 0 };
    unsigned char colorFlags                = 0;
    unsigned char padding                   = 0;
};

struct RainbowZoneData : ZoneData
{
    unsigned char cycle_or_led_num          = 20;
};

struct FeaturePacket_185
{
    const unsigned char report_id           = 0x52; // Report ID
    ZoneData            j_rgb_1;                    // 1
    ZoneData            j_pipe_1;                   // 11
    ZoneData            j_pipe_2;                   // 21
    RainbowZoneData     j_rainbow_1;                // 31
    RainbowZoneData     j_rainbow_2;                // 42
    CorsairZoneData     j_corsair;                  // 53
    ZoneData            j_corsair_outerll120;       // 64
    ZoneData            on_board_led;               // 74
    ZoneData            on_board_led_1;             // 84
    ZoneData            on_board_led_2;             // 94
    ZoneData            on_board_led_3;             // 104
    ZoneData            on_board_led_4;             // 114
    ZoneData            on_board_led_5;             // 124
    ZoneData            on_board_led_6;             // 134
    ZoneData            on_board_led_7;             // 144
    ZoneData            on_board_led_8;             // 154
    ZoneData            on_board_led_9;             // 164
    ZoneData            j_rgb_2;                    // 174
    unsigned char       save_data           = 0;    // 184
};


class MSIMysticLight185Controller
{
public:
    MSIMysticLight185Controller(hid_device* handle, const char *path);
    ~MSIMysticLight185Controller();

    unsigned int    GetZoneMinLedCount
                        (
                        MSI_ZONE        zone
                        );

    unsigned int    GetZoneMaxLedCount
                        (
                        MSI_ZONE        zone
                        );

    unsigned int    GetZoneLedCount
                        (
                        MSI_ZONE        zone
                        );

    void            SetZoneLedCount
                        (
                        MSI_ZONE        zone,
                        unsigned int    led_count
                        );

    void            SetMode
                        (
                        MSI_ZONE        zone,
                        MSI_MODE        mode,
                        MSI_SPEED       speed,
                        MSI_BRIGHTNESS  brightness,
                        bool            rainbow_color
                        );

    void            GetMode
                        (
                        MSI_ZONE        zone,
                        MSI_MODE        &mode,
                        MSI_SPEED       &speed,
                        MSI_BRIGHTNESS  &brightness,
                        bool            &rainbow_color
                        );

    void            SetZoneColor
                        (
                        MSI_ZONE        zone,
                        unsigned char   red1,
                        unsigned char   grn1,
                        unsigned char   blu1,
                        unsigned char   red2,
                        unsigned char   grn2,
                        unsigned char   blu2
                        );

    void            SetCycleCount
                        (
                        MSI_ZONE        zone,
                        unsigned char   cycle_num
                        );

    unsigned char   GetCycleCount
                        (
                        MSI_ZONE        zone
                        );

    std::pair<Color, Color>
                    GetZoneColor(MSI_ZONE zone);

    bool            Update();

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    std::string     GetFWVersion();
    std::string     GetSerial();

private:
    bool            ReadSettings();
    void            SaveOnUpdate(bool send);
    bool            ReadFwVersion();
    void            ReadSerial();
    void            ReadName();
    ZoneData*       GetZoneData(MSI_ZONE zone);
    RainbowZoneData*
                    GetRainbowZoneData(MSI_ZONE zone);
    static unsigned char   BitSet(unsigned char value, bool bit, unsigned int position);

    hid_device*             dev;
    std::string             name;
    std::string             location;
    std::string             version_APROM;
    std::string             version_LDROM;
    std::string             chip_id;

    FeaturePacket_185       data;
};

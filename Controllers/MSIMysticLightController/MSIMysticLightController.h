/*-----------------------------------------*\
|  MSIMysticLightController.h               |
|                                           |
|  Definitions and types for MSI Mystic     |
|   Light USB lighting controllers          |
|                                           |
|  T-bond 3/4/2020                          |
\*-----------------------------------------*/

#include "RGBController.h"
#include <cstring>
#include <hidapi/hidapi.h>
#include <limits>

#pragma once

enum ZONE
{
    J_RGB_1 = 1,
    J_RGB_2 = 174,
    J_PIPE_1 = 11,
    J_PIPE_2 = 21,
    J_RAINBOW_1 = 31,
    J_RAINBOW_2 = 42,
    J_CORSAIR = 53,
    J_CORSAIR_OUTERLL120 = 64,
    ON_BOARD_LED = 74,
    ON_BOARD_LED_1 = 84,
    ON_BOARD_LED_2 = 94,
    ON_BOARD_LED_3 = 104,
    ON_BOARD_LED_4 = 114,
    ON_BOARD_LED_5 = 124,
    ON_BOARD_LED_6 = 134,
    ON_BOARD_LED_7 = 144,
    ON_BOARD_LED_8 = 154,
    ON_BOARD_LED_9 = 164
};

struct ZoneDescription
{
    std::string name;
    ZONE value;
};

enum EFFECT
{
    DISABLE,
    STATIC,
    BREATHING,
    FLASHING,
    DOUBLE_FLASHING,
    LIGHTNING,
    MSI_MARQUEE,
    METEOR,
    WATER_DROP,
    MSI_RAINBOW,
    POP,
    RAP,
    JAZZ,
    PLAY,
    MOVIE,
    COLOR_RING,
    PLANETARY,
    DOUBLE_METEOR,
    ENERGY,
    BLINK,
    CLOCK,
    COLOR_PULSE,
    COLOR_SHIFT,
    COLOR_WAVE,
    MARQUEE,
    RAINBOW,
    RAINBOW_WAVE,
    VISOR,
    JRAINBOW,
    RAINBOW_FLASHING,
    RAINBOW_DOUBLE_FLASHING,
    RANDOM,
    FAN_CONTROL,
    DISABLE_2,
    COLOR_RING_FLASHING,
    COLOR_RING_DOUBLE_FLASHING,
    STACK,
    CORSAIR_QUE,
    FIRE,
    LAVA
};

enum SPEED
{
    LOW,
    MEDIUM,
    HIGH
};

enum FAN_TYPE
{
    SP,
    HD,
    LL
};

enum BRIGHTNESS
{
    OFF,
    LEVEL_10,
    LEVEL_20,
    LEVEL_30,
    LEVEL_40,
    LEVEL_50,
    LEVEL_60,
    LEVEL_70,
    LEVEL_80,
    LEVEL_90,
    LEVEL_100
};

struct Color
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

struct CorsairZoneData
{
    unsigned char effect = EFFECT::STATIC;
    Color color { std::numeric_limits<unsigned char>::max(), 0, 0 };
    unsigned char fan_flags = 40;
    unsigned char corsair_quantity;
    unsigned char padding[3];

    unsigned char is_individual = 0;
};

struct ZoneData
{
    unsigned char effect = EFFECT::STATIC;
    Color color { std::numeric_limits<unsigned char>::max(), 0u, 0u };
    unsigned char speedAndBrightnessFlags = 40u;
    Color color2 { 0, std::numeric_limits<unsigned char>::max(), 0u };
    unsigned char colorFlags = 128u;

    const unsigned char padding = 0u;
};

struct RainbowZoneData : ZoneData
{
    unsigned char cycle_or_led_num = 20u;
};

struct FeaturePacket
{
    const unsigned char report_id = 82u; // Report ID
    ZoneData j_rgb_1; // 1
    ZoneData j_pipe_1; // 11
    ZoneData j_pipe_2; // 21
    RainbowZoneData j_rainbow_1; // 31
    RainbowZoneData j_rainbow_2; // 42
    CorsairZoneData j_corsair; // 53
    ZoneData j_corsair_outerll120; // 64
    ZoneData on_board_led; // 74
    ZoneData on_board_led_1; // 84
    ZoneData on_board_led_2; // 94
    ZoneData on_board_led_3; // 104
    ZoneData on_board_led_4; // 114
    ZoneData on_board_led_5; // 124
    ZoneData on_board_led_6; // 134
    ZoneData on_board_led_7; // 144
    ZoneData on_board_led_8; // 154
    ZoneData on_board_led_9; // 164
    ZoneData j_rgb_2; // 174

    unsigned char save_data = 0u; // 184
};


class MSIMysticLightController
{
public:
    MSIMysticLightController(hid_device* handle, const char *path);
    ~MSIMysticLightController();

    unsigned int    GetZoneMinLedCount(ZONE zone);
    unsigned int    GetZoneMaxLedCount(ZONE zone);
    unsigned int    GetZoneLedCount(ZONE zone);
    void            SetZoneLedCount(ZONE zone, unsigned int led_count);

    void            SetMode(ZONE zone, EFFECT mode, SPEED speed, BRIGHTNESS brightness, bool rainbow_color);
    void            GetMode(ZONE zone, EFFECT &mode, SPEED &speed, BRIGHTNESS &brightness, bool &rainbow_color);
    void            SetZoneColor(ZONE zone, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2);
    void            SetCycleCount(ZONE zone, unsigned char cycle_num);
    unsigned char   GetCycleCount(ZONE zone);
    std::pair<Color, Color>
                    GetZoneColor(ZONE zone);
    bool            Update();

    void            SetDeviceSettings(bool stripe_or_fan, FAN_TYPE fan_type, unsigned char corsair_device_quantity, bool is_LL120Outer_individual);
    void            GetDeviceSettings(bool &stripe_or_fan, FAN_TYPE &fan_type, unsigned char &corsair_device_quantity, bool &is_LL120Outer_individual);
    bool            SetVolume(unsigned char main, unsigned char left, unsigned char right);
    void            SetBoardSyncSettings(bool onboard_sync, bool combine_JRGB, bool combine_JPIPE1, bool combine_JPIPE2, bool combine_JRAINBOW1, bool combine_JRAINBOW2, bool combine_crossair);
    void            GetBoardSyncSettings(bool &onboard_sync, bool &combine_JRGB, bool &combine_JPIPE1, bool &combine_JPIPE2, bool &combine_JRAINBOW1, bool &combine_JRAINBOW2, bool &combine_crossair);

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
    ZoneData*       GetZoneData(ZONE zone);
    RainbowZoneData*
                    GetRainbowZoneData(ZONE zone);
    static unsigned char   BitSet(unsigned char value, bool bit, unsigned int position);

    hid_device*             dev;
    std::string             name;
    std::string             loc;
    std::string             version_APROM;
    std::string             version_LDROM;
    std::string             chip_id;

    FeaturePacket data;
};

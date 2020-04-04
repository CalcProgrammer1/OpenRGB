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

struct ZoneData
{
    unsigned char effect = EFFECT::STATIC;
    Color color { std::numeric_limits<unsigned char>::max(), 0, 0 };
    unsigned char speedAndBrightnessFlags = 40;
    Color color2 { 0, std::numeric_limits<unsigned char>::max(), 0 };
    unsigned char colorFlags = 128;

    const unsigned char padding = 0;
};

struct RainbowZoneData : ZoneData
{
    unsigned char cycleNum = 20;
};

struct FeaturePacket
{
    const unsigned char report_id = 82; // Report ID
    ZoneData j_rgb_1; // 1
    ZoneData j_pipe_1; // 11
    ZoneData j_pipe_2; // 21
    RainbowZoneData j_rainbow_1; // 31
    RainbowZoneData j_rainbow_2; // 42
    RainbowZoneData j_corsair; // 53
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

    unsigned char send_led_data = 0; // 184
};


class MSIMysticLightController
{
public:
    MSIMysticLightController(hid_device* handle, const char *path);
    ~MSIMysticLightController();

    unsigned int    GetZoneMinLedCount(ZONE zone);
    unsigned int    GetZoneMaxLedCount(ZONE zone);
    unsigned int    GetZoneLedCount(ZONE zone);

    void            SetMode(ZONE zone, EFFECT mode, SPEED speed, BRIGHTNESS brightness, bool rainbow_color);
    void            SetZoneColor(ZONE zone, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2);
    bool            Update();

    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    std::string     GetFWVersion();
    std::string     GetSerial();

private:
    bool            UpdateController();
    void            SetSendLedSettings(bool send);
    ZoneData*       GetZoneData(ZONE zone);

    hid_device*             dev;
    std::string             name;
    std::string             loc;
    std::string             version;
    std::string             chip_id;

    FeaturePacket data;
};

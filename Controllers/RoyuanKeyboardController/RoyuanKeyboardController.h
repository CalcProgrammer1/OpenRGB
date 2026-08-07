/*---------------------------------------------------------*\
| RoyuanKeyboardController.h                                |
|                                                           |
|   Driver for ROYUAN-based keyboard lighting controllers   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <hidapi.h>

#define ROYUAN_REPORT_LENGTH                                      64
#define ROYUAN_HID_MAX_STR                                        255

#define ROYUAN_COMMAND_SET_LED_PARAMS                             0x07
#define ROYUAN_COMMAND_GET_LED_PARAMS                             0x87
#define ROYUAN_COMMAND_SET_DIRECT_COLOR                           0x0E
#define ROYUAN_COMMAND_SET_USERPIC                                0x0C

#define ROYUAN_SKYLOONG_GK68HE_PRO_USERPIC_SLOTS                  128
#define ROYUAN_SKYLOONG_GK68HE_PRO_USERPIC_FRAMES                 7

#define ROYUAN_AKKO_EFFECT_OFF                                    0x00
#define ROYUAN_AKKO_EFFECT_STATIC                                 0x01
#define ROYUAN_AKKO_EFFECT_BREATHING                              0x02
#define ROYUAN_AKKO_EFFECT_WAVE                                   0x04
#define ROYUAN_AKKO_EFFECT_RIPPLE                                 0x05
#define ROYUAN_AKKO_EFFECT_REACTIVE                               0x08

#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_ALWAYS_ON               0x01
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_DYNAMIC_BREATHING       0x02
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_SPECTRUM_CYCLE          0x03
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_WAVE                    0x04
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_RIPPLE                  0x05
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_STARLIGHT               0x06
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_STREAM                  0x07
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_SHADOW                  0x08
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_MOUNTAIN_WAVE           0x09
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_SINE_WAVE               0x0A
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_COLOR_SPRING            0x0B
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_FLOWER_WAVE             0x0C
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_CUSTOM                  0x0D
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_KILL_TWO_BIRDS          0x0E
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_CIRCLE_WAVE             0x0F
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_COLORFUL_CROSS          0x10
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_SNOW_FALL               0x11
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_METEOR                  0x12
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_TRACELESS_SNOWSTEP      0x13
#define ROYUAN_SKYLOONG_GK68HE_PRO_EFFECT_DIRECT                  0x15

#define ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_USER_COLOR              0x07
#define ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_RAINBOW                 0x08
#define ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_1_DEFAULT       0x00
#define ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_2               0x10
#define ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_3               0x20
#define ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_4               0x30
#define ROYUAN_SKYLOONG_GK68HE_PRO_OPTION_PATTERN_5               0x40

#define ROYUAN_AKKO_SPEED_MIN                                     0x00
#define ROYUAN_AKKO_SPEED_MAX                                     0x04
#define ROYUAN_AKKO_SPEED_DEFAULT                                 0x00

#define ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MIN                      0x00
#define ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_MAX                      0x04
#define ROYUAN_SKYLOONG_GK68HE_PRO_SPEED_DEFAULT                  0x02

#define ROYUAN_AKKO_BRIGHTNESS_MIN                                0x00
#define ROYUAN_AKKO_BRIGHTNESS_MAX                                0x04
#define ROYUAN_AKKO_BRIGHTNESS_DEFAULT                            0x03

#define ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MIN                 0x00
#define ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_MAX                 0x04
#define ROYUAN_SKYLOONG_GK68HE_PRO_BRIGHTNESS_DEFAULT             0x04

enum class RoyuanKeyboardProtocol
{
    Legacy,
    AkkoBSeries
};

enum class RoyuanChecksumType
{
    NextPowerOfTwo,
    OnesComplement
};

struct RoyuanKeyboardProfile
{
    RoyuanKeyboardProtocol protocol;
    RoyuanChecksumType     checksum_type;
    unsigned char          speed_max;
    unsigned char          speed_default;
    unsigned char          brightness_max;
    unsigned char          brightness_default;
    unsigned char          option_default;
    unsigned char          solid_option_default;
    unsigned char          dazzle_default;
    unsigned char          direct_mode_id;
    bool                   reverse_speed;
    bool                   supports_readback;
    bool                   supports_direct_mode;
    bool                   solid_option_overrides_static;
    bool                   force_option_default;
    std::string            fallback_name;

    static RoyuanKeyboardProfile EpomakerLegacy();
    static RoyuanKeyboardProfile AkkoBSeries();
    static RoyuanKeyboardProfile GK68HEPro();
};

class RoyuanKeyboardController
{
public:
    RoyuanKeyboardController(hid_device* dev_handle, char* path, const RoyuanKeyboardProfile& profile);
    virtual ~RoyuanKeyboardController();

    bool                    IsValid() const;

    std::string             GetDeviceName();
    std::string             GetSerial();
    std::string             GetLocation();

    RoyuanKeyboardProtocol  GetProtocol() const;
    unsigned char           GetMode() const;
    unsigned char           GetSpeed() const;
    unsigned char           GetBrightness() const;
    unsigned char           GetDirectModeID() const;
    unsigned char           GetRed() const;
    unsigned char           GetGreen() const;
    unsigned char           GetBlue() const;

    void                    SetDazzle(bool is_dazzle);
    void                    SetOption(unsigned char option);
    void                    SetMode(unsigned char mode, unsigned char speed, unsigned char brightness);
    void                    SetColor(unsigned char red, unsigned char green, unsigned char blue);
    bool                    SetUserPic(const unsigned char* slot_colors, unsigned char layer);

private:
    std::string             device_name;
    std::string             location;
    hid_device*             dev;
    RoyuanKeyboardProfile   profile;
    bool                    valid;

    unsigned char           current_mode;
    unsigned char           current_speed;
    unsigned char           current_brightness;
    unsigned char           current_dazzle;
    unsigned char           current_option;
    unsigned char           current_red;
    unsigned char           current_green;
    unsigned char           current_blue;

    unsigned char           CalculateChecksum(const unsigned char* data, std::size_t length) const;
    bool                    ReadState();
    bool                    SendUpdate();
    bool                    SendDirectColor();
};

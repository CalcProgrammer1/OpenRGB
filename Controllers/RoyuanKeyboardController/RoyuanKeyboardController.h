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

#define ROYUAN_REPORT_LENGTH                         64
#define ROYUAN_HID_MAX_STR                           255

#define ROYUAN_COMMAND_SET_LED_PARAMS                0x07
#define ROYUAN_COMMAND_GET_LED_PARAMS                0x87

#define ROYUAN_AKKO_EFFECT_OFF                       0x00
#define ROYUAN_AKKO_EFFECT_STATIC                    0x01
#define ROYUAN_AKKO_EFFECT_BREATHING                 0x02
#define ROYUAN_AKKO_EFFECT_WAVE                      0x04
#define ROYUAN_AKKO_EFFECT_RIPPLE                    0x05
#define ROYUAN_AKKO_EFFECT_REACTIVE                  0x08

#define ROYUAN_AKKO_SPEED_MIN                        0x00
#define ROYUAN_AKKO_SPEED_MAX                        0x04
#define ROYUAN_AKKO_SPEED_DEFAULT                    0x00

#define ROYUAN_AKKO_BRIGHTNESS_MIN                   0x00
#define ROYUAN_AKKO_BRIGHTNESS_MAX                   0x04
#define ROYUAN_AKKO_BRIGHTNESS_DEFAULT               0x03

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
    unsigned char          dazzle_default;
    bool                   reverse_speed;
    bool                   supports_readback;
    std::string            fallback_name;

    static RoyuanKeyboardProfile EpomakerLegacy();
    static RoyuanKeyboardProfile AkkoBSeries();
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
    unsigned char           GetRed() const;
    unsigned char           GetGreen() const;
    unsigned char           GetBlue() const;

    void                    SetDazzle(bool is_dazzle);
    void                    SetOption(unsigned char option);
    void                    SetMode(unsigned char mode, unsigned char speed, unsigned char brightness);
    void                    SetColor(unsigned char red, unsigned char green, unsigned char blue);

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
};

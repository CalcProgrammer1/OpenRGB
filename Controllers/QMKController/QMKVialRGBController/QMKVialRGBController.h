/*---------------------------------------------------------*\
| QMKVialRGBController.h                                    |
|                                                           |
|   Driver for VialRGB QMK Keyboard Protocol                |
|                                                           |
|   Adam Honse <calcprogrammer1@gmail.com)      29 Sep 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "ResourceManager.h"
#include "RGBController.h"

#define MSG_LEN                         32

enum
{
    CMD_GET_PROTOCOL_VERSION            = 0x01,
    CMD_GET_KEYBOARD_VALUE              = 0x02,
    CMD_SET_KEYBOARD_VALUE              = 0x03,
    CMD_VIA_DYNAMIC_KEYMAP_GET_KEYCODE  = 0x04,
    CMD_VIA_DYNAMIC_KEYMAP_SET_KEYCODE  = 0x05,
    CMD_VIA_DYNAMIC_KEYMAP_RESET        = 0x06,
    CMD_LIGHTING_SET_VALUE              = 0x07,
    CMD_LIGHTING_GET_VALUE              = 0x08,
    CMD_VIAL_COMMAND                    = 0xFE,
};

enum
{
    VIAL_GET_KEYBOARD_ID                = 0x00,
    VIAL_GET_SIZE                       = 0x01,
    VIAL_GET_DEFINITION                 = 0x02,
    VIAL_GET_UNLOCK_STATUS              = 0x05,
    VIAL_UNLOCK_START                   = 0x06,
    VIAL_UNLOCK_POLL                    = 0x07,
    VIAL_LOCK                           = 0x08,
    VIALRGB_GET_INFO                    = 0x40,
    VIALRGB_GET_MODE                    = 0x41,
    VIALRGB_GET_SUPPORTED               = 0x42,
    VIALRGB_GET_NUMBER_LEDS             = 0x43,
    VIALRGB_GET_LED_INFO                = 0x44,
    VIALRGB_SET_MODE                    = 0x41,
    VIALRGB_DIRECT_FASTSET              = 0x42,
};

enum
{
    VIALRGB_EFFECT_OFF,
    VIALRGB_EFFECT_DIRECT,
    VIALRGB_EFFECT_SOLID_COLOR,
    VIALRGB_EFFECT_ALPHAS_MODS,
    VIALRGB_EFFECT_GRADIENT_UP_DOWN,
    VIALRGB_EFFECT_GRADIENT_LEFT_RIGHT,
    VIALRGB_EFFECT_BREATHING,
    VIALRGB_EFFECT_BAND_SAT,
    VIALRGB_EFFECT_BAND_VAL,
    VIALRGB_EFFECT_BAND_PINWHEEL_SAT,
    VIALRGB_EFFECT_BAND_PINWHEEL_VAL,
    VIALRGB_EFFECT_BAND_SPIRAL_SAT,
    VIALRGB_EFFECT_BAND_SPIRAL_VAL,
    VIALRGB_EFFECT_CYCLE_ALL,
    VIALRGB_EFFECT_CYCLE_LEFT_RIGHT,
    VIALRGB_EFFECT_CYCLE_UP_DOWN,
    VIALRGB_EFFECT_RAINBOW_MOVING_CHEVRON,
    VIALRGB_EFFECT_CYCLE_OUT_IN,
    VIALRGB_EFFECT_CYCLE_OUT_IN_DUAL,
    VIALRGB_EFFECT_CYCLE_PINWHEEL,
    VIALRGB_EFFECT_CYCLE_SPIRAL,
    VIALRGB_EFFECT_DUAL_BEACON,
    VIALRGB_EFFECT_RAINBOW_BEACON,
    VIALRGB_EFFECT_RAINBOW_PINWHEELS,
    VIALRGB_EFFECT_RAINDROPS,
    VIALRGB_EFFECT_JELLYBEAN_RAINDROPS,
    VIALRGB_EFFECT_HUE_BREATHING,
    VIALRGB_EFFECT_HUE_PENDULUM,
    VIALRGB_EFFECT_HUE_WAVE,
    VIALRGB_EFFECT_TYPING_HEATMAP,
    VIALRGB_EFFECT_DIGITAL_RAIN,
    VIALRGB_EFFECT_SOLID_REACTIVE_SIMPLE,
    VIALRGB_EFFECT_SOLID_REACTIVE,
    VIALRGB_EFFECT_SOLID_REACTIVE_WIDE,
    VIALRGB_EFFECT_SOLID_REACTIVE_MULTIWIDE,
    VIALRGB_EFFECT_SOLID_REACTIVE_CROSS,
    VIALRGB_EFFECT_SOLID_REACTIVE_MULTICROSS,
    VIALRGB_EFFECT_SOLID_REACTIVE_NEXUS,
    VIALRGB_EFFECT_SOLID_REACTIVE_MULTINEXUS,
    VIALRGB_EFFECT_SPLASH,
    VIALRGB_EFFECT_MULTISPLASH,
    VIALRGB_EFFECT_SOLID_SPLASH,
    VIALRGB_EFFECT_SOLID_MULTISPLASH,
    VIALRGB_EFFECT_PIXEL_RAIN,
    VIALRGB_EFFECT_PIXEL_FRACTAL,
    VIALRGB_EFFECT_SKIP                         = 0xFFFF
};

typedef struct
{
    unsigned char   x;
    unsigned char   y;
    unsigned char   flags;
    unsigned char   row;
    unsigned char   col;
} vialrgb_led_info;

class QMKVialRGBController
{
public:
    QMKVialRGBController(hid_device *dev_handle, const char *path);
    ~QMKVialRGBController();

    std::string         GetLocation();
    std::string         GetName();
    std::string         GetSerial();
    std::string         GetVendor();
    std::string         GetVersion();

    unsigned short      GetEffect(std::size_t effect_idx);
    std::size_t         GetEffectCount();
    unsigned short      GetKeycode(unsigned short led_index);
    unsigned short      GetLEDCount();
    vialrgb_led_info    GetLEDInfo(unsigned short led_index);

    void GetMode
        (
        unsigned short*     mode,
        unsigned char*      speed,
        unsigned char*      hue,
        unsigned char*      sat,
        unsigned char*      val
        );

    void SendLEDs
        (
        unsigned short      number_leds,
        RGBColor*           color_data
        );

    void SetMode
        (
        unsigned short      mode,
        unsigned char       speed,
        unsigned char       hue,
        unsigned char       sat,
        unsigned char       val
        );

private:
    hid_device*                     dev;
    unsigned long long              keyboard_uid;
    std::vector<unsigned short>     keycodes;
    std::vector<vialrgb_led_info>   led_info;
    std::string                     location;
    unsigned char                   maximum_brightness;
    std::string                     name;
    unsigned short                  number_leds;
    std::string                     serial;
    std::vector<unsigned short>     supported_effects;
    std::string                     vendor;
    unsigned short                  via_protocol_version;
    unsigned int                    vial_protocol_version;
    unsigned short                  vialrgb_protocol_version;
    unsigned char                   vialrgb_flag;

    unsigned short CmdGetKeycode
        (
        unsigned char       layer,
        unsigned char       row,
        unsigned char       col
        );

    vialrgb_led_info CmdGetLEDInfo
        (
        unsigned short      led_index
        );

    void CmdGetMode
        (
        unsigned short*     mode,
        unsigned char*      speed,
        unsigned char*      hue,
        unsigned char*      sat,
        unsigned char*      val
        );

    void CmdGetNumberLEDs
        (
        unsigned short*     number_leds
        );

    void CmdGetSupportedEffects();

    void CmdGetVialInfo
        (
        unsigned int*       vial_protocol_version,
        unsigned long long* keyboard_uid,
        unsigned char*      vialrgb_flag
        );

    void CmdGetVialRGBInfo
        (
        unsigned short*     vialrgb_protocol_version,
        unsigned char*      maximum_brightness
        );

    void CmdGetViaProtocolVersion
        (
        unsigned short*     via_protocol_version
        );

    void CmdSendLEDs
        (
        unsigned short      start_index,
        unsigned char       number_leds,
        RGBColor*           color_data
        );

    void CmdSetMode
        (
        unsigned short      mode,
        unsigned char       speed,
        unsigned char       hue,
        unsigned char       sat,
        unsigned char       val
        );

    int SendCommand
        (
        unsigned char       cmd,
        unsigned char       subcmd,
        unsigned char*      data_in,
        unsigned char       data_in_size,
        unsigned char*      data_out,
        unsigned char       data_out_size
        );

    int SendCheckCommand
        (
        unsigned char       cmd,
        unsigned char       subcmd,
        unsigned char*      data_in,
        unsigned char       data_in_size,
        unsigned char*      data_out,
        unsigned char       data_out_size
        );
};

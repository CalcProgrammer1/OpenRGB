/*---------------------------------------------------------*\
| CMKeyboardV3Controller.h                                  |
|                                                           |
|   Driver for Cooler Master V3 keyboards                   |
|   Derived from CMKeyboardV2Controller.h                   |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|   Wikus van Rooyen                            14 Nov 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "CMKeyboardAbstractController.h"

#define CM_KEYBOARD_V3_WIRED_WRITE_SIZE     65
#define CM_KEYBOARD_V3_WIRED_PAYLOAD_SIZE   62
#define CM_KEYBOARD_V3_DONGLE_WRITE_SIZE    33
#define CM_KEYBOARD_V3_DONGLE_PAYLOAD_SIZE  20
#define CM_KEYBOARD_V3_DONGLE_DELAY         20
#define CM_KEYBOARD_V3_READ_TIMEOUT         200
#define CM_KEYBOARD_V3_LED_COUNT            71
#define CM_KEYBOARD_V3_PROFILE_SIZE         0xC9
#define CM_KEYBOARD_V3_PROFILE_COUNT        4
#define CM_KEYBOARD_V3_PROFILE_REG          0x324
#define CM_KEYBOARD_V3_EFFECT_SIZE          10
#define CM_KEYBOARD_V3_EFFECT_MODE_REG      0xC8

enum cm_keyboard_v3_command
{
    CM_KB_V3_CMD_WRITE_REG                  = 0x01,
    CM_KB_V3_CMD_READ_REG                   = 0x02,
    CM_KB_V3_CMD_WRITE_CUSTOM_RGB           = 0x0A,
};

enum cm_keyboard_v3_effect
{
    CM_KB_V3_STATIC                         = 0x00,
    CM_KB_V3_WAVE                           = 0x01,
    CM_KB_V3_CROSS                          = 0x02,
    CM_KB_V3_REACTIVE_FADE                  = 0x03,
    CM_KB_V3_CUSTOM                         = 0x04,
    CM_KB_V3_STARS                          = 0x05,
    CM_KB_V3_SNOW                           = 0x06,
    CM_KB_V3_CYCLE                          = 0x07,
    CM_KB_V3_BREATHE                        = 0x08,
    CM_KB_V3_RIPPLE                         = 0x09,
    CM_KB_V3_REACTIVE_PUNCH                 = 0x0A,
    CM_KB_V3_HEARTBEAT                      = 0x0B,
    CM_KB_V3_FIREBALL                       = 0x0C,
    CM_KB_V3_CIRCLE_SPECTRUM                = 0x0D,
    CM_KB_V3_REACTIVE_TORNADO               = 0x0E,
    CM_KB_V3_WATER_RIPPLE                   = 0x0F,
    CM_KB_V3_OFF                            = 0x13,
    CM_KB_V3_NONE                           = 0xFF,
};

struct stCMKeyboardV3_mode
{
    const char      *name;
    unsigned int    value;
    unsigned int    speed_min;
    unsigned int    speed_max;
    unsigned int    speed;
    unsigned int    brightness;
    unsigned int    direction;
    unsigned int    nColors;
    unsigned int    color_mode;
    unsigned int    flags;
};

class CMKeyboardV3Controller : public CMKeyboardAbstractController
{
public:
    CMKeyboardV3Controller(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name);
    ~CMKeyboardV3Controller();

    /*---------------------------------------------------------*\
    | Protocol specific functions to be implemented             |
    \*---------------------------------------------------------*/
    void            SetLeds(std::vector<led> leds, std::vector<RGBColor> colors);
    void            SetSingleLED(uint8_t in_led, RGBColor in_color);
    void            Initialize();
    void            Shutdown();
    void            SetLEDControl(bool bManual);
    void            SetCustomMode();
    void            SetMode(mode selectedMode);
    void            InitializeModes(std::vector<mode> &modes);
    KEYBOARD_LAYOUT GetKeyboardLayout();

private:
    void            _SetEffectMode(uint8_t effectId);
    void            _ReadActiveProfile();
    void            _WriteRegister(uint16_t address, std::vector<uint8_t> data);
    void            _SendCommand(uint8_t command, std::vector<uint8_t> data);

    uint8_t         m_profileId;
    size_t          m_writeSize;
    size_t          m_payloadSize;
    bool            m_bDongle;
};

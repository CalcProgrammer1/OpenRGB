/*---------------------------------------------------------*\
| AsusAuraCoreLaptopController.h                            |
|                                                           |
|   Driver for ASUS ROG Aura Core Laptop                    |
|                                                           |
|   Chris M (Dr_No)                             28 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "LogManager.h"
#include "RGBController.h"
#include "ResourceManager.h"
#include "RGBControllerKeyNames.h"
#include "AsusAuraCoreLaptopDevices.h"

#define NA                                      0xFFFFFFFF
#define HID_MAX_STR                             255
#define ASUSAURACORELAPTOP_TIMEOUT              250
#define ASUSAURACORELAPTOP_READ_PACKET_SIZE     64
#define ASUSAURACORELAPTOP_WRITE_PACKET_SIZE    64   //Buffer requires a prepended ReportID hence + 1

#define ASUSAURACORELAPTOP_KEYCOUNT             91
#define ASUSAURACORELAPTOP_KEY_WIDTH            18
#define ASUSAURACORELAPTOP_KEY_HEIGHT           7
#define ASUSAURACORELAPTOP_LIGHTBARCOUNT        6
#define ASUSAURACORELAPTOP_LIDCOUNT             3
#define ASUSAURACORELAPTOP_POWER_ZONES          4
#define ASUSAURACORELAPTOP_POWER_STATES         4
#define ASUSAURACORELAPTOP_BRIGHTNESS_MIN       0
#define ASUSAURACORELAPTOP_BRIGHTNESS_MAX       3 // No device has proven to have 256 keyboard brightness levels, only 0..3

enum
{
    ASUSAURACORELAPTOP_MODE_OFF                 = 0x00, //Turn off - All leds off
    ASUSAURACORELAPTOP_MODE_DIRECT              = 0xFF, //Direct Led Control - Independently set LEDs in zone
    ASUSAURACORELAPTOP_MODE_STATIC              = 0x00, //Static Mode - Set entire zone to a single color.
    ASUSAURACORELAPTOP_MODE_BREATHING           = 0x01, //Breathing Mode - Fades between fully off and fully on.
    ASUSAURACORELAPTOP_MODE_SPECTRUM            = 0x02, //Spectrum Cycle Mode - Cycles through the color spectrum on all lights on the device
    ASUSAURACORELAPTOP_MODE_RAINBOW             = 0x03, //Rainbow Wave Mode - Cycle thru the color spectrum as a wave across all LEDs
    ASUSAURACORELAPTOP_MODE_FLASHING            = 0x0A, //Flashing Mode - Abruptly changing between fully off and fully on.

    /*-------------------------------------------------*\
    | Modes not implemented in the Armoury Crate        |
    |   OEM software that were discovered.              |
    \*-------------------------------------------------*/
    ASUSAURACORELAPTOP_MODE_STARRY_NIGHT        = 0x04, //Starry Night Mode
    ASUSAURACORELAPTOP_MODE_RAIN                = 0x05, //Rain Mode
    ASUSAURACORELAPTOP_MODE_REACT_FADE          = 0x06, //Reactive Fade Mode
    ASUSAURACORELAPTOP_MODE_REACT_LASER         = 0x07, //Reactive Laser Mode
    ASUSAURACORELAPTOP_MODE_REACT_RIPPLE        = 0x08, //Reactive Ripple Mode
    ASUSAURACORELAPTOP_MODE_COMET               = 0x0B, //Comet Mode
    ASUSAURACORELAPTOP_MODE_FLASHNDASH          = 0x0C, //Flash n Dash Mode
    ASUSAURACORELAPTOP_MODE_KEYSTONE            = 0x0D, //Keystone Mode
};

enum
{
    ASUSAURACORELAPTOP_ZONE_BYTE                = 2,
    ASUSAURACORELAPTOP_MODE_BYTE                = 3,
    ASUSAURACORELAPTOP_R1_BYTE                  = 4,
    ASUSAURACORELAPTOP_G1_BYTE                  = 5,
    ASUSAURACORELAPTOP_B1_BYTE                  = 6,
    ASUSAURACORELAPTOP_SPEED_BYTE               = 7,
    ASUSAURACORELAPTOP_DIRECTION_BYTE           = 8,
    ASUSAURACORELAPTOP_DATA_BYTE                = 9,
    ASUSAURACORELAPTOP_R2_BYTE                  = 10,
    ASUSAURACORELAPTOP_G2_BYTE                  = 11,
    ASUSAURACORELAPTOP_B2_BYTE                  = 12,
};

enum
{
    ASUSAURACORELAPTOP_REPORT_ID                = 0x5D,
    ASUSAURACORELAPTOP_CMD_BRIGHTNESS           = 0xBA,
    ASUSAURACORELAPTOP_CMD_DIRECT               = 0xBC,
    ASUSAURACORELAPTOP_CMD_POWER                = 0xBD,
    ASUSAURACORELAPTOP_CMD_UPDATE               = 0xB3,
    ASUSAURACORELAPTOP_CMD_APPLY                = 0xB4,
    ASUSAURACORELAPTOP_CMD_SET                  = 0xB5,
    ASUSAURACORELAPTOP_CMD_LAYOUT               = 0x05,
};

enum
{
    ASUSAURACORELAPTOP_SPEED_SLOWEST            = 0xE1, // Slowest speed
    ASUSAURACORELAPTOP_SPEED_NORMAL             = 0xEB, // Normal speed
    ASUSAURACORELAPTOP_SPEED_FASTEST            = 0xF5, // Fastest speed
};

enum aura_core_laptop_layout
{
    ASUSAURACORELAPTOP_LAYOUT_ANSI              = 0x01,     /* US ANSI Layout                   */
    ASUSAURACORELAPTOP_LAYOUT_ISO               = 0x02,     /* EURO ISO Layout                  */
};

struct p_state
{
    std::string zone;
    bool        state;
};

class AsusAuraCoreLaptopController
{
public:
    AsusAuraCoreLaptopController(hid_device* dev_handle, const char* path);
    ~AsusAuraCoreLaptopController();

    const aura_core_laptop_device*  GetDeviceData();
    std::string                     GetDeviceDescription();
    std::string                     GetSerial();
    unsigned int                    GetKeyboardLayout();
    std::string                     GetLocation();

    void                            SetMode(uint8_t mode, uint8_t speed, uint8_t brightness, RGBColor color1, RGBColor color2, uint8_t random, uint8_t direction);
    void                            SetLedsDirect(std::vector<RGBColor *>colors);
    void                            SendInitDirectMode();
private:
    hid_device*                     dev;
    uint16_t                        device_index;
    std::string                     location;

    uint8_t                         current_mode;
    uint8_t                         current_speed;
    uint8_t                         current_direction;

    RGBColor                        current_c1;
    RGBColor                        current_c2;
    uint8_t                         current_brightness;
    uint8_t                         current_random;

    void                            SendApply();
    void                            SendBrightness();
    void                            SendSet();
    void                            SendUpdate();

    void                            SetPowerConfigFromJSON();
    void                            SendPowerConfig(uint32_t flags);
    uint32_t                        PackPowerFlags(bool flags[]);
    std::vector<p_state>            PowerConfigArray();
};

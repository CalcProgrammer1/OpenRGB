/*---------------------------------------------------------*\
| MSIMonitorController.h                                    |
|                                                           |
|   Driver for MSI monitor (gaming controller)              |
|                                                           |
|   Andy Herbert                              2026 June 1   |
|   Ken Sanislo                               2026 July 20  |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define MSI_MONITOR_LEDS 9
#define MSI_MONITOR_PACKET_SIZE 78

/*---------------------------------------------------------*\
| 0x72 layout (dual control block, e.g. MPG 322URX QD-OLED) |
\*---------------------------------------------------------*/
#define MSI_MONITOR_72_PACKET_SIZE      168
#define MSI_MONITOR_72_STORE_INDEX      167
#define MSI_MONITOR_72_ARRAY_A_INDEX    23
#define MSI_MONITOR_72_ARRAY_B_INDEX    95
#define MSI_MONITOR_72_ARRAY_SLOTS      24

/*---------------------------------------------------------*\
| Fill order the firmware expects (vendor Direction enum).  |
| Only the physical origin differs; index 0 is that end.    |
| The controller mirrors RightToLeft into left origin so    |
| OpenRGB LED 0 is always the left/start.  Circle and Ctype |
| are non linear shapes: driven as a plain line for now,    |
| they want a custom layout once someone maps the shape.    |
\*---------------------------------------------------------*/
enum
{
    MSI_MONITOR_72_DIR_RIGHT_TO_LEFT = 0,
    MSI_MONITOR_72_DIR_LEFT_TO_RIGHT = 1,
    MSI_MONITOR_72_DIR_UP_TO_DOWN    = 2,
    MSI_MONITOR_72_DIR_CIRCLE        = 3,
    MSI_MONITOR_72_DIR_CTYPE         = 4,
};

/*---------------------------------------------------------*\
| Layout version query rides the short report channel:      |
| write [0x01][opcode], read [0x01][0x5A][data]             |
\*---------------------------------------------------------*/
#define MSI_MONITOR_QUERY_PACKET_SIZE   64
#define MSI_MONITOR_QUERY_REPORT_ID     0x01
#define MSI_MONITOR_QUERY_ACK           0x5A
#define MSI_MONITOR_OPCODE_GET_VERSION  0xB0
#define MSI_MONITOR_VERSION_UNKNOWN     0

enum
{
    MSI_MONITOR_OFF_MODE_VALUE       = 0x00,
    MSI_MONITOR_STATIC_MODE_VALUE    = 0x01,
    MSI_MONITOR_BREATHING_MODE_VALUE = 0x02,
    MSI_MONITOR_FLASHING_MODE_VALUE  = 0x03,
    MSI_MONITOR_LIGHTNING_MODE_VALUE = 0x05,
    MSI_MONITOR_MARQUEE_MODE_VALUE   = 0x06,
    MSI_MONITOR_METEOR_MODE_VALUE    = 0x08,
    MSI_MONITOR_RAINBOW_MODE_VALUE   = 0x1A,
    MSI_MONITOR_RANDOM_MODE_VALUE    = 0x1F
};

/*---------------------------------------------------------*\
| 0x72 layout mode values.  These match the MSI Mystic      |
| Light motherboard mode enum value for value (see          |
| MSIMysticLightController/MSIMysticLightCommon.h): the     |
| monitor runs the same Nuvoton Mystic Light engine, so the |
| effect numbering is shared with its motherboard sibling.  |
| Values 10-14 and 32 are the audio modes, not exposed      |
| here (they need the music level feed).                    |
\*---------------------------------------------------------*/
enum
{
    MSI_MONITOR_72_MODE_OFF              = 0,
    MSI_MONITOR_72_MODE_STATIC           = 1,
    MSI_MONITOR_72_MODE_BREATHING        = 2,
    MSI_MONITOR_72_MODE_FLASHING         = 3,
    MSI_MONITOR_72_MODE_DOUBLE_FLASHING  = 4,
    MSI_MONITOR_72_MODE_LIGHTNING        = 5,
    MSI_MONITOR_72_MODE_SCANNER          = 6,
    MSI_MONITOR_72_MODE_METEOR           = 7,
    MSI_MONITOR_72_MODE_WATER_DROP       = 8,
    MSI_MONITOR_72_MODE_RAINBOW_MSI      = 9,
    MSI_MONITOR_72_MODE_COLOR_RING       = 15,
    MSI_MONITOR_72_MODE_PLANETARY        = 16,
    MSI_MONITOR_72_MODE_DOUBLE_METEOR    = 17,
    MSI_MONITOR_72_MODE_ENERGY           = 18,
    MSI_MONITOR_72_MODE_BLINK            = 19,
    MSI_MONITOR_72_MODE_CLOCK            = 20,
    MSI_MONITOR_72_MODE_COLOR_PULSE      = 21,
    MSI_MONITOR_72_MODE_COLOR_SHIFT      = 22,
    MSI_MONITOR_72_MODE_COLOR_WAVE       = 23,
    MSI_MONITOR_72_MODE_MARQUEE          = 24,
    MSI_MONITOR_72_MODE_RAINBOW          = 25,
    MSI_MONITOR_72_MODE_RAINBOW_WAVE     = 26,
    MSI_MONITOR_72_MODE_VISOR            = 27,
    MSI_MONITOR_72_MODE_JRAINBOW         = 28,
    MSI_MONITOR_72_MODE_RAINBOW_FLASH    = 29,
    MSI_MONITOR_72_MODE_RAINBOW_DBLFLASH = 30,
    MSI_MONITOR_72_MODE_RAINBOW_LINE     = 33,
};

enum
{
    MSI_MONITOR_72_SPEED_LOW    = 0,
    MSI_MONITOR_72_SPEED_MEDIUM = 1,
    MSI_MONITOR_72_SPEED_HIGH   = 2,
};

class MSIMonitorController
{
public:
    MSIMonitorController(hid_device *dev_handle, const hid_device_info &info, std::string dev_name);
    ~MSIMonitorController();

    std::string GetDeviceLocation();
    std::string GetFirmwareVersion();
    std::string GetNameString();
    std::string GetSerialString();

    void Set(uint8_t mode_value, const std::vector<RGBColor> colors, uint8_t last_bit);

    uint8_t GetLayoutVersion();
    void SetMode72(uint8_t mode_value, uint8_t speed, uint8_t brightness, RGBColor color1, RGBColor color2, bool user_palette, bool save, const std::vector<RGBColor>& led_colors, bool fill_both_arrays);

private:
    hid_device *dev;
    std::string description;
    std::string location;
    std::string name;
    std::string version;
};

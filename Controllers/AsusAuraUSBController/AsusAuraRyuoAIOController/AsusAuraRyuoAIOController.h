/*---------------------------------------------------------*\
| AsusAuraRyuoAIOController.h                               |
|                                                           |
|   Driver for ASUS Aura Ryuo                               |
|                                                           |
|   Cooper Hall (geobot19 / Geo_bot)            08 Apr 2022 |
|   using snipets from Chris M (Dr.No)                      |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "AsusAuraUSBController.h"

#define ASUSAURARYUOAIOCONTROLLER_TIMEOUT        250
#define ASUSAURARYUOAIOCONTROLLER_HID_MAX_STR    255

#define ASUSAURARYUOAIOCONTROLLER_BRIGHTNESS_MIN 0
#define ASUSAURARYUOAIOCONTROLLER_BRIGHTNESS_MAX 255

class AsusAuraRyuoAIOController : public AuraUSBController
{
public:
    enum
    {
        MODE_DIRECT       = 0xFF, //Direct Led Control - Independently set LEDs in zone
        MODE_STATIC       = 0x01, //Static Mode - Set entire zone to a single color.
        MODE_BREATHING    = 0x02, //Breathing Mode - Fades between fully off and fully on.
        MODE_FLASHING     = 0x03, //Flashing Mode - Abruptly changing between fully off and fully on.
        MODE_SPECTRUM     = 0x04, //Spectrum Cycle Mode - Cycles through the color spectrum on all lights on the device
        MODE_RAINBOW      = 0x05, //Rainbow Wave Mode - Cycle thru the color spectrum as a wave across all LEDs
        MODE_FLASHANDDASH = 0x0A, //Flash n Dash - Flash twice and then flash in direction
    };

    enum PacketMap
    {
        REPORTID_BYTE  = 0,
        COMMAND_BYTE   = 1,
        ZONE_BYTE      = 2,
        PROGRAM_ID_BYTE= 3,
        MODE_BYTE      = 4,
        RED_BYTE       = 5,
        GREEN_BYTE     = 6,
        BLUE_BYTE      = 7,
        DIRECTION_BYTE = 8,
        SPEED_BYTE     = 9,
    };

    enum
    {
        SPEED_SLOWEST     = 0x04, // Slowest speed
        SPEED_SLOW        = 0x03, // Slower speed
        SPEED_NORMAL      = 0x02, // Normal speed
        SPEED_FAST        = 0x01, // Fast speed
        SPEED_FASTEST     = 0x00, // Fastest speed
    };

    AsusAuraRyuoAIOController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~AsusAuraRyuoAIOController();

    std::string             GetLocation();

    void                    SetChannelLEDs(unsigned char channel, RGBColor *colors, unsigned int num_colors);
    void                    SetLedsDirect(RGBColor * led_colours, uint8_t led_count);

    void                    SetMode(unsigned char channel, unsigned char mode, unsigned char red, unsigned char grn, unsigned char blu);
    void                    SetMode(unsigned char mode, unsigned char speed, unsigned char direction, RGBColor colour);
private:
    static const uint8_t    read_packet_size    = 65;
    static const uint8_t    write_packet_size   = 65;
    static const uint8_t    modefx   = 0x3B;
    static const uint8_t    direct   = 0x40;
    static const uint8_t    reportid = 0xEC;
    static const uint8_t    program_id = 0x22;

    std::string             location;

    uint8_t                 zone_index;
    uint8_t                 current_mode;
    uint8_t                 current_speed;

    uint8_t                 current_red;
    uint8_t                 current_green;
    uint8_t                 current_blue;
    uint8_t                 current_direction;

    void                    GetStatus();
    void                    SendUpdate();
    void                    SendEffect(unsigned char channel, unsigned char mode, unsigned char red, unsigned char grn, unsigned char blu);
    void                    SendDirectApply(unsigned char channel);
};

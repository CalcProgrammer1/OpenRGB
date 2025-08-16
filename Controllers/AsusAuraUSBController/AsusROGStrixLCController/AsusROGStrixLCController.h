/*---------------------------------------------------------*\
| AsusROGStrixLCController.h                                |
|                                                           |
|   Driver for ASUS Aura liquid cooler                      |
|                                                           |
|   Chris M (Dr_No)                             17 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"
#include "AsusAuraUSBController.h"

#define ROGSTRIXLC_CONTROLLER_TIMEOUT 250
#define HID_MAX_STR 255

#define ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MIN  0
#define ROGSTRIXLC_CONTROLLER_BRIGHTNESS_MAX  255

enum
{
    ROGSTRIXLC_CONTROLLER_MODE_DIRECT       = 0xFF, //Direct Led Control - Independently set LEDs in zone
    ROGSTRIXLC_CONTROLLER_MODE_STATIC       = 0x01, //Static Mode - Set entire zone to a single color.
    ROGSTRIXLC_CONTROLLER_MODE_BREATHING    = 0x02, //Breathing Mode - Fades between fully off and fully on.
    ROGSTRIXLC_CONTROLLER_MODE_FLASHING     = 0x03, //Flashing Mode - Abruptly changing between fully off and fully on.
    ROGSTRIXLC_CONTROLLER_MODE_SPECTRUM     = 0x04, //Spectrum Cycle Mode - Cycles through the color spectrum on all lights on the device
    ROGSTRIXLC_CONTROLLER_MODE_RAINBOW      = 0x05, //Rainbow Wave Mode - Cycle thru the color spectrum as a wave across all LEDs
    ROGSTRIXLC_CONTROLLER_MODE_FLASHANDDASH = 0x0A, //Flash n Dash - Flash twice and then flash in direction
};

enum AsusAuraStrixLC_PacketMap
{
    ROGSTRIXLC_REPORTID_BYTE                = 0,
    ROGSTRIXLC_COMMAND_BYTE                 = 1,
    ROGSTRIXLC_ZONE_BYTE                    = 2,
    ROGSTRIXLC_MODE_BYTE                    = 3,
    ROGSTRIXLC_RED_BYTE                     = 4,
    ROGSTRIXLC_GREEN_BYTE                   = 5,
    ROGSTRIXLC_BLUE_BYTE                    = 6,
    ROGSTRIXLC_DIRECTION_BYTE               = 7,
    ROGSTRIXLC_SPEED_BYTE                   = 8,
};

enum
{
    ROGSTRIXLC_CONTROLLER_SPEED_SLOWEST     = 0x04, // Slowest speed
    ROGSTRIXLC_CONTROLLER_SPEED_SLOW        = 0x03, // Slower speed
    ROGSTRIXLC_CONTROLLER_SPEED_NORMAL      = 0x02, // Normal speed
    ROGSTRIXLC_CONTROLLER_SPEED_FAST        = 0x01, // Fast speed
    ROGSTRIXLC_CONTROLLER_SPEED_FASTEST     = 0x00, // Fastest speed
};

class AsusROGStrixLCController : public AuraUSBController
{
public:
    AsusROGStrixLCController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~AsusROGStrixLCController();

    std::string             GetLocation();

    void                    SetChannelLEDs(unsigned char channel, RGBColor *colors, unsigned int num_colors);
    void                    SetLedsDirect(RGBColor * led_colours, uint8_t led_count);

    void                    SetMode(unsigned char channel, unsigned char mode, unsigned char red, unsigned char grn, unsigned char blu);
    void                    SetMode(unsigned char mode, unsigned char speed, unsigned char direction, RGBColor colour);
private:
    static const uint8_t    read_packet_size    = 64;
    static const uint8_t    write_packet_size   = read_packet_size + 1;
    static const uint8_t    rogstrixlc_modefx   = 0x3B;
    static const uint8_t    rogstrixlc_direct   = 0x40;
    static const uint8_t    rogstrixlc_reportid = 0xEC;

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

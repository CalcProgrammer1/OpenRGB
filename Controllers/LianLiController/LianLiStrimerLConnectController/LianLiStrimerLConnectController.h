/*---------------------------------------------------------*\
| LianLiStrimerLConnectController.h                         |
|                                                           |
|   Driver for Lian Li Strimer L Connect                    |
|                                                           |
|   Chris M (Dr_No)                             03 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "LogManager.h"
#include "RGBController.h"

#define HID_MAX_STR 255
#define STRIMERLCONNECT_PACKET_SIZE     255 //Buffer requires a prepended ReportID hence + 1

#define STRIMERLCONNECT_BRIGHTNESS_MIN  0   //Brightness indexes not values
#define STRIMERLCONNECT_BRIGHTNESS_MAX  4
#define STRIMERLCONNECT_STRIP_COUNT     12

enum
{
    STRIMERLCONNECT_MODE_OFF            = 0x00, //Turn off - All leds off
    STRIMERLCONNECT_MODE_DIRECT         = 0x01, //Direct Led Control - Independently set LEDs in zone
    STRIMERLCONNECT_MODE_BREATHING      = 0x02, //Breathing Mode - Fades between fully off and fully on.
    STRIMERLCONNECT_MODE_FLASHING       = 0x03, //Flashing Mode - Abruptly changing between fully off and fully on.
    STRIMERLCONNECT_MODE_RAINBOWMORPH   = 0x04, //Rainbow Morph Mode
    STRIMERLCONNECT_MODE_RAINBOW        = 0x05, //Rainbow Wave Mode - Cycle thru the color spectrum as a wave across all LEDs
    STRIMERLCONNECT_MODE_BREATHCYCLE    = 0x06, //Spectrum Cycle Mode - Cycles through the color spectrum on all lights on the device

    STRIMERLCONNECT_MODE_SNOOKER        = 0x19, //Snooker Mode
    STRIMERLCONNECT_MODE_MIXING         = 0x1A, //Mixing Mode
    STRIMERLCONNECT_MODE_PINGPONG       = 0x1B, //Ping Pong Mode
    STRIMERLCONNECT_MODE_RUNWAY         = 0x1C, //Runway Mode
    STRIMERLCONNECT_MODE_PAINTING       = 0x1D, //Painting Mode
    STRIMERLCONNECT_MODE_TIDE           = 0x1E, //Tide Mode
    STRIMERLCONNECT_MODE_BLOWUP         = 0x1F, //Blow Up Mode
    STRIMERLCONNECT_MODE_METEOR         = 0x20, //Meteor Mode

    STRIMERLCONNECT_MODE_SHOCKWAVE      = 0x21, //Shock Wave Mode
    STRIMERLCONNECT_MODE_RIPPLE         = 0x22, //Ripple Mode
    STRIMERLCONNECT_MODE_VOICE          = 0x23, //Voice Mode
    STRIMERLCONNECT_MODE_BULLETSTACK    = 0x24, //Bullet Stack Mode
    STRIMERLCONNECT_MODE_DRIZZLING      = 0x25, //Drizzling Mode
    STRIMERLCONNECT_MODE_FADEOUT        = 0x26, //Fade Out Mode
    STRIMERLCONNECT_MODE_COLORTRANSFER  = 0x27, //Color Transfer Mode
    STRIMERLCONNECT_MODE_CROSSOVER      = 0x28, //Cross Over Mode
    STRIMERLCONNECT_MODE_TWINKLE        = 0x29, //Twinkle Mode
    STRIMERLCONNECT_MODE_CONTEST        = 0x2A, //Contest Mode
    STRIMERLCONNECT_MODE_PARALLEL       = 0x2B, //Parallel Mode
};

enum
{
    STRIMERLCONNECT_COMMAND_BYTE        = 1,
    STRIMERLCONNECT_DATA_BYTE           = 2,
    STRIMERLCONNECT_SPEED_BYTE          = 3,
    STRIMERLCONNECT_DIRECTION_BYTE      = 4,
    STRIMERLCONNECT_BRIGHTNESS_BYTE     = 5,

    STRIMERLCONNECT_MODE_COMMAND        = 0x10,
    STRIMERLCONNECT_COLOUR_COMMAND      = 0x30,
    STRIMERLCONNECT_REPORT_ID           = 0xE0,
};

enum
{
    STRIMERLCONNECT_SPEED_SLOWEST       = 0,
    STRIMERLCONNECT_SPEED_NORMAL        = 2,
    STRIMERLCONNECT_SPEED_FASTEST       = 4,
};

class LianLiStrimerLConnectController
{
public:
    LianLiStrimerLConnectController(hid_device* dev_handle, const char* path);
    ~LianLiStrimerLConnectController();

    std::string     GetDeviceName();
    std::string     GetSerial();
    std::string     GetLocation();

    void            SendApply();
    void            SetMode(uint8_t mode, uint8_t zone, uint8_t speed, uint8_t brightness, uint8_t direction, bool random_colours);
    void            SetLedsDirect(uint8_t zone, RGBColor *led_colours, uint8_t led_count);
private:
    std::string     device_name;
    std::string     location;
    hid_device*     dev;
};

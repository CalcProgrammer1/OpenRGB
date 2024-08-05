/*---------------------------------------------------------*\
| AsusAuraCoreController.h                                  |
|                                                           |
|   Driver for ASUS ROG Aura Core                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

enum AuraCoreDeviceType
{
    AURA_CORE_DEVICE_UNKNOWN            = 0,
    AURA_CORE_DEVICE_KEYBOARD           = 1,
    AURA_CORE_DEVICE_GA15DH             = 2
};

enum
{
    AURA_CORE_MODE_STATIC               =   0,      /* Static color mode                    */
    AURA_CORE_MODE_BREATHING            =   1,      /* Breathing effect mode                */
    AURA_CORE_MODE_SPECTRUM_CYCLE       =   2,      /* Spectrum Cycle mode                  */
    AURA_CORE_MODE_RAINBOW              =   3,      /* Rainbow mode                         */
    AURA_CORE_MODE_STROBE               =  10,      /* Strobe mode                          */
    AURA_CORE_MODE_COMET                =  11,      /* Comet mode                           */
    AURA_CORE_MODE_FLASHNDASH           =  12,      /* Flash & Dash mode                    */
    AURA_CORE_MODE_IRRADIATION          =  17,      /* Irradiation mode                     */
    AURA_CORE_MODE_DIRECT               = 255       /* Not a real mode - but need a way to differentiate */
};

enum
{
    AURA_CORE_COMMAND_UPDATE            = 0xB3,     /* Update mode and color                */
    AURA_CORE_COMMAND_SET               = 0xB5,     /* Set command                          */
    AURA_CORE_COMMAND_APPLY             = 0xB4,     /* Apply command                        */
    AURA_CORE_COMMAND_BRIGHTNESS        = 0xBA,     /* Brightness command                   */
    AURA_CORE_COMMAND_DIRECT            = 0xBC      /* Set LEDs directly                    */
};

enum
{
    AURA_CORE_ZONE_IDX_ALL              = 0x00,     /* Update all zones                     */
    AURA_CORE_ZONE_IDX_1                = 0x01,     /* Update zone 1                        */
    AURA_CORE_ZONE_IDX_2                = 0x02,     /* Update zone 2                        */
    AURA_CORE_ZONE_IDX_3                = 0x03,     /* Update zone 3                        */
    AURA_CORE_ZONE_IDX_4                = 0x04,     /* Update zone 4                        */
};

enum
{
    AURA_CORE_SPEED_SLOW                = 0xE1,     /* Slowest speed                        */
    AURA_CORE_SPEED_NORMAL              = 0xEB,     /* Normal speed                         */
    AURA_CORE_SPEED_FAST                = 0xF5,     /* Fastest speed                        */
};

struct AuraDeviceDescriptor
{
    AuraCoreDeviceType  aura_type;
    unsigned char       buff_size;
    unsigned char       report_id;
    unsigned char       num_leds;
    unsigned char       max_leds_per_message;
    bool                supports_direct;
};

struct AuraColor
{
    unsigned char   red;
    unsigned char   green;
    unsigned char   blue;
};

class AuraCoreController
{
public:
    AuraDeviceDescriptor    aura_device;

    AuraCoreController(hid_device* dev_handle, const char* path);
    ~AuraCoreController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void    SendBrightness
                (
                unsigned char   brightness
                );

    void    SendUpdate
                (
                unsigned char   zone,
                unsigned char   mode,
                unsigned char   speed,
                unsigned char   dir,
                unsigned char   red,
                unsigned char   green,
                unsigned char   blue
                );

    void    SendSet();

    void    SendApply();

    void    InitDirectMode();

    void    UpdateDirect(std::vector<AuraColor>& color_set);

private:
    hid_device*             dev;
    std::string             location;

    void    IdentifyDevice();
    void    Handshake();
    void    SendIdString();
    void    SendQuery();
};

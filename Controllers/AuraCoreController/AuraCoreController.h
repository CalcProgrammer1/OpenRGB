/*-----------------------------------------*\
|  AuraCoreController.h                     |
|                                           |
|  Definitions and types for ASUS ROG Aura  |
|  Core RGB lighting controller             |
|                                           |
|  Adam Honse (CalcProgrammer1) 4/13/2020   |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    AURA_CORE_MODE_STATIC               = 0,        /* Static color mode                    */
    AURA_CORE_MODE_BREATHING            = 1,        /* Breathing effect mode                */
    AURA_CORE_MODE_SPECTRUM_CYCLE       = 2,        /* Spectrum Cycle mode                  */
};

enum
{
    AURA_CORE_COMMAND_UPDATE            = 0xB3,     /* Update mode and color                */
    AURA_CORE_COMMAND_SET               = 0xB5,     /* Set command                          */
    AURA_CORE_COMMAND_APPLY             = 0xB4,     /* Apply command                        */
    AURA_CORE_COMMAND_BRIGHTNESS        = 0xBA,     /* Brightness command                   */
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

class AuraCoreController
{
public:
    AuraCoreController(hid_device* dev_handle);
    ~AuraCoreController();

    void    SendBrightness
                (
                unsigned char   brightness
                );

    void    SendUpdate
                (
                unsigned char   zone,
                unsigned char   mode,
                unsigned char   speed,
                unsigned char   red,
                unsigned char   green,
                unsigned char   blue
                );

    void    SendSet();

    void    SendApply();

private:
    hid_device*             dev;

};

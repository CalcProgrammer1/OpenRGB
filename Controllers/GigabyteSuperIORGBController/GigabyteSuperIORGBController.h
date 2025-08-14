/*---------------------------------------------------------*\
| GigabyteSuperIORGBController.h                            |
|                                                           |
|   Driver for Gigabyte Aorus Super IO motherboard          |
|                                                           |
|   Ryan Frankcombe (422gRdHuX5uk)              11 Sep 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>

enum
{
    /*--------------------------------*\
    | Chip Entry Registers and Values  |
    \*_-------------------------------*/
    GIGABYTE_SUPERIO_CHIPENTRY_REGISTER_1           = 0x01,
    GIGABYTE_SUPERIO_CHIPENTRY_REGISTER_2           = 0x55,
    GIGABYTE_SUPERIO_CHIPENTRY_VALUE_1              = 0x00,
    GIGABYTE_SUPERIO_CHIPENTRY_VALUE_2              = 0x00,

    /*--------------------------------*\
    | Chip Select Registers and Values |
    \*_-------------------------------*/
    GIGABYTE_SUPERIO_CHIPSELECT_REGISTER_1          = 0x07,
    GIGABYTE_SUPERIO_CHIPSELECT_VALUE_1             = 0x00,

    /*--------------------------------*\
    | Chip Exit Registers and Values   |
    \*_-------------------------------*/
    GIGABYTE_SUPERIO_CHIPEXIT_REGISTER_1            = 0x02,
    GIGABYTE_SUPERIO_CHIPEXIT_VALUE_1               = 0x01,
};

enum
{
    /*--------------------------------*\
    | Chip Color Registers             |
    \*_-------------------------------*/
    GIGABYTE_SUPERIO_RED_REGISTER_1        = 0xB3,
    GIGABYTE_SUPERIO_RED_REGISTER_2        = 0xC3,
    GIGABYTE_SUPERIO_GREEN_REGISTER_1      = 0xB4,
    GIGABYTE_SUPERIO_GREEN_REGISTER_2      = 0xC4,
    GIGABYTE_SUPERIO_BLUE_REGISTER_1       = 0xB5,
    GIGABYTE_SUPERIO_BLUE_REGISTER_2       = 0xC5,
};

enum
{
    /*--------------------------------*\
    | Chip Modes                       |
    \*_-------------------------------*/
    GIGABYTE_MODE1_STATIC                   = 0x00,     /* Mode 1 static effect                */
    GIGABYTE_MODE1_RAINBOW                  = 0x01,     /* Mode 1 rainbow effect               */
    GIGABYTE_MODE1_BREATHING                = 0x02,     /* Mode 1 breathing effect             */
    GIGABYTE_MODE1_FLASHING                 = 0x03,      /* Mode 1 flashing effect              */

    /*--------------------------------*\
    | Chip Static Mode Registers       |
    \*_-------------------------------*/
    GIGABYTE_SUPERIO_STATIC_REGISTER_1      = 0xB0,
    GIGABYTE_SUPERIO_STATIC_REGISTER_2      = 0xB1,
    GIGABYTE_SUPERIO_STATIC_REGISTER_3      = 0xB2,
    GIGABYTE_SUPERIO_STATIC_REGISTER_4      = 0xC0,
    GIGABYTE_SUPERIO_STATIC_REGISTER_5      = 0xC1,
    GIGABYTE_SUPERIO_STATIC_REGISTER_6      = 0xC2,
    GIGABYTE_SUPERIO_STATIC_VALUE_1         = 0x0F,
    GIGABYTE_SUPERIO_STATIC_VALUE_2         = 0x00,
    GIGABYTE_SUPERIO_STATIC_VALUE_3         = 0x20,
    GIGABYTE_SUPERIO_STATIC_VALUE_4         = 0x0F,
    GIGABYTE_SUPERIO_STATIC_VALUE_5         = 0x00,
    GIGABYTE_SUPERIO_STATIC_VALUE_6         = 0x10,

    /*--------------------------------*\
    | Chip Rainbow Mode Registers      |
    \*_-------------------------------*/
    GIGABYTE_SUPERIO_RAINBOW_REGISTER_1     = 0xB0,
    GIGABYTE_SUPERIO_RAINBOW_REGISTER_2     = 0xB1,
    GIGABYTE_SUPERIO_RAINBOW_REGISTER_3     = 0xB2,
    GIGABYTE_SUPERIO_RAINBOW_REGISTER_4     = 0xB0,
    GIGABYTE_SUPERIO_RAINBOW_REGISTER_5     = 0xC0,
    GIGABYTE_SUPERIO_RAINBOW_REGISTER_6     = 0xC1,
    GIGABYTE_SUPERIO_RAINBOW_REGISTER_7     = 0xC2,
    GIGABYTE_SUPERIO_RAINBOW_REGISTER_8     = 0xC0,
    GIGABYTE_SUPERIO_RAINBOW_VALUE_1        = 0x00,
    GIGABYTE_SUPERIO_RAINBOW_VALUE_2        = 0x00,
    GIGABYTE_SUPERIO_RAINBOW_VALUE_3        = 0xA0,
    GIGABYTE_SUPERIO_RAINBOW_VALUE_4        = 0x7F,
    GIGABYTE_SUPERIO_RAINBOW_VALUE_5        = 0x00,
    GIGABYTE_SUPERIO_RAINBOW_VALUE_6        = 0x00,
    GIGABYTE_SUPERIO_RAINBOW_VALUE_7        = 0x90,
    GIGABYTE_SUPERIO_RAINBOW_VALUE_8        = 0x7F,

    /*--------------------------------*\
    | Chip Breathing Mode Registers    |
    \*_-------------------------------*/
    GIGABYTE_SUPERIO_BREATHING_REGISTER_1   = 0xB0,
    GIGABYTE_SUPERIO_BREATHING_REGISTER_2   = 0xB1,
    GIGABYTE_SUPERIO_BREATHING_REGISTER_3   = 0xB2,
    GIGABYTE_SUPERIO_BREATHING_REGISTER_4   = 0xC0,
    GIGABYTE_SUPERIO_BREATHING_REGISTER_5   = 0xC1,
    GIGABYTE_SUPERIO_BREATHING_REGISTER_6   = 0xC2,
    GIGABYTE_SUPERIO_BREATHING_VALUE_1      = 0x8F,
    GIGABYTE_SUPERIO_BREATHING_VALUE_2      = 0x00,
    GIGABYTE_SUPERIO_BREATHING_VALUE_3      = 0x20,
    GIGABYTE_SUPERIO_BREATHING_VALUE_4      = 0x8F,
    GIGABYTE_SUPERIO_BREATHING_VALUE_5      = 0x00,
    GIGABYTE_SUPERIO_BREATHING_VALUE_6      = 0x10,

    /*--------------------------------*\
    | Chip Flashing Mode Registers     |
    \*_-------------------------------*/
    GIGABYTE_SUPERIO_FLASHING_REGISTER_1    = 0xB0,
    GIGABYTE_SUPERIO_FLASHING_REGISTER_2    = 0xB1,
    GIGABYTE_SUPERIO_FLASHING_REGISTER_3    = 0xB2,
    GIGABYTE_SUPERIO_FLASHING_REGISTER_4    = 0xC0,
    GIGABYTE_SUPERIO_FLASHING_REGISTER_5    = 0xC1,
    GIGABYTE_SUPERIO_FLASHING_REGISTER_6    = 0xC2,
    GIGABYTE_SUPERIO_FLASHING_VALUE_1       = 0x0F,
    GIGABYTE_SUPERIO_FLASHING_VALUE_2       = 0x08,
    GIGABYTE_SUPERIO_FLASHING_VALUE_3       = 0x20,
    GIGABYTE_SUPERIO_FLASHING_VALUE_4       = 0x0F,
    GIGABYTE_SUPERIO_FLASHING_VALUE_5       = 0x08,
    GIGABYTE_SUPERIO_FLASHING_VALUE_6       = 0x10,

};

class GigabyteSuperIORGBController
{
public:
    GigabyteSuperIORGBController(int sioaddr, std::string dev_name);
    ~GigabyteSuperIORGBController();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();

    unsigned int    GetMode();
    void            SetMode(int new_mode);

    void            SetColor(unsigned int red, unsigned int green, unsigned int blue);
    void            ChipEntry();
    void            ChipExit();
private:
    int             gig_sioaddr;
    std::string     name;
};

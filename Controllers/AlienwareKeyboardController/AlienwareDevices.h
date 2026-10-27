/*---------------------------------------------------------*\
| AlienwareDevices.h                                        |
|                                                           |
|   Device table for Alienware keyboards                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include "RGBController.h"
#include "RGBControllerKeyNames.h"

/*---------------------------------------------------------*\
| Alienware vendor ID                                       |
\*---------------------------------------------------------*/
#define ALIENWARE_VID                               0x04F2

/*---------------------------------------------------------*\
| Keyboard product IDs                                      |
\*---------------------------------------------------------*/
#define ALIENWARE_AW410K_PID                        0x1968
#define ALIENWARE_AW510K_PID                        0x1830

/*---------------------------------------------------------*\
| Maximum columns in the keyboard matrix                    |
\*---------------------------------------------------------*/
#define ALIENWARE_MAX_COLS                          24

/*---------------------------------------------------------*\
| Zone IDs (shared protocol)                                |
\*---------------------------------------------------------*/
enum
{
    ALIENWARE_ZONE_MODE_KEYBOARD                    = 0x01,
    ALIENWARE_ZONE_MODE_LOGO                        = 0x07,
};

enum
{
    ALIENWARE_ZONE_DIRECT_KEYBOARD                  = 0x01,
    ALIENWARE_ZONE_DIRECT_MEDIA                     = 0x02,
    ALIENWARE_ZONE_DIRECT_LOGO                      = 0x07,
    ALIENWARE_ZONE_DIRECT_INDICATORS                 = 0x40,
};

/*---------------------------------------------------------*\
| Mode IDs (shared protocol)                                |
\*---------------------------------------------------------*/
enum
{
    ALIENWARE_MODE_OFF                              = 0x00,
    ALIENWARE_MODE_DIRECT                           = 0x01,
    ALIENWARE_MODE_DIRECT_PER_LED                   = 0x01,
    ALIENWARE_MODE_PULSE                            = 0x02,
    ALIENWARE_MODE_MORPH                            = 0x03,
    ALIENWARE_MODE_BREATHING                        = 0x07,
    ALIENWARE_MODE_SPECTRUM                         = 0x08,
    ALIENWARE_MODE_SINGLE_WAVE                      = 0x0F,
    ALIENWARE_MODE_RAINBOW_WAVE                     = 0x10,
    ALIENWARE_MODE_SCANNER                          = 0x11,
    ALIENWARE_MODE_STATIC                           = 0x13,
};

enum
{
    ALIENWARE_SPEED_SLOWEST                         = 0x2D,
    ALIENWARE_SPEED_NORMAL                          = 0x19,
    ALIENWARE_SPEED_FASTEST                         = 0x0A,
};

enum
{
    ALIENWARE_DIRECTION_LEFT_TO_RIGHT               = 0x01,
    ALIENWARE_DIRECTION_RIGHT_TO_LEFT               = 0x02,
    ALIENWARE_DIRECTION_TOP_TO_BOTTOM               = 0x03,
    ALIENWARE_DIRECTION_BOTTOM_TO_TOP               = 0x04,
};

enum
{
    ALIENWARE_SINGLE_COLOR_MODE                     = 0x01,
    ALIENWARE_TWO_USER_DEFINED_COLOR_MODE           = 0x02,
    ALIENWARE_RAINBOW_COLOR_MODE                    = 0x03,
};

/*---------------------------------------------------------*\
| LED table entry                                           |
\*---------------------------------------------------------*/
typedef struct
{
    const char*         name;
    unsigned char       zone;
    unsigned char       idx;
} alienware_led;

/*---------------------------------------------------------*\
| Device table entry                                        |
\*---------------------------------------------------------*/
typedef struct
{
    std::string         name;
    unsigned short      pid;
    unsigned int        rows;
    unsigned int        cols;
    const unsigned int  (*matrix_map)[ALIENWARE_MAX_COLS];
    unsigned int        zone_size;
    const char*         zone_name;
    const alienware_led* leds;
    unsigned int        led_count;
    bool                has_logo;
} alienware_device;

const alienware_device* GetAlienwareDevice(unsigned short pid);

/*---------------------------------------------------------*\
| Device table (defined in AlienwareDevices.cpp)            |
\*---------------------------------------------------------*/
extern const unsigned int ALIENWARE_NUM_DEVICES;
extern const alienware_device* alienware_device_list[];

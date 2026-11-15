/*---------------------------------------------------------*\
| NZXTHue2Devices.h                                         |
|                                                           |
|   Device list for NZXT Hue 2 devices                      |
|                                                           |
|   Ferrah Aiko (ferrahwolfeh@proton.me)        12 Nov 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>

/*---------------------------------------------------------*\
| NZXT vendor ID                                            |
\*---------------------------------------------------------*/
#define NZXT_VID                                0x1E71

/*---------------------------------------------------------*\
| NZXT Hue 2 product IDs                                    |
\*---------------------------------------------------------*/
#define NZXT_HUE_2_PID                          0x2001
#define NZXT_HUE_2_AMBIENT_PID                  0x2002
#define NZXT_MOTHERBOARD_DEVICE_PID             0x2005
#define NZXT_MOTHERBOARD_DEVICE_2_PID           0x200B
#define NZXT_SMART_DEVICE_V2_PID                0x2006
#define NZXT_KRAKEN_X3_SERIES_PID               0x2007
#define NZXT_KRAKEN_X3_SERIES_RGB_PID           0x2014
#define NZXT_KRAKEN_2024_ELITE_SERIES_RGB_PID   0x3012
#define NZXT_KRAKEN_2024_PLUS_PID               0x3014
#define NZXT_RGB_FAN_CONTROLLER_PID             0x2009
#define NZXT_RGB_FAN_CONTROLLER2_PID            0x2010
#define NZXT_RGB_FAN_CONTROLLER3_PID            0x200E
#define NZXT_RGB_FAN_CONTROLLER4_PID            0x2011
#define NZXT_RGB_FAN_CONTROLLER5_PID            0x2019
#define NZXT_RGB_FAN_CONTROLLER6_PID            0x2020
#define NZXT_RGB_FAN_CONTROLLER7_PID            0x201F
#define NZXT_RGB_FAN_CONTROLLER8_PID            0x2022
#define NZXT_RGB_FAN_CONTROLLER9_PID            0x201B
#define NZXT_RGB_CONTROLLER_1_PID               0x2012
#define NZXT_RGB_CONTROLLER_2_PID               0x2021
#define NZXT_SMART_DEVICE_V2_1_PID              0x200D
#define NZXT_SMART_DEVICE_V2_2_PID              0x200F

/*---------------------------------------------------------*\
| NZXT Hue 2 maximum channels and packet sizes              |
\*---------------------------------------------------------*/
#define NZXT_HUE_2_MAX_CHANNELS                 6
#define NZXT_HUE_2_PACKET_SIZE_64               64
#define NZXT_HUE_2_PACKET_SIZE_512              512
#define NZXT_HUE_2_MAX_PACKET_SIZE              512

/*---------------------------------------------------------*\
| NZXT Hue 2 protocol variants                              |
\*---------------------------------------------------------*/
typedef enum
{
    NZXT_HUE_2_PROTOCOL_LEGACY,
    NZXT_HUE_2_PROTOCOL_2023,
    NZXT_HUE_2_PROTOCOL_2024_PLUS,
} nzxt_hue2_protocol;

typedef struct
{
    std::string                         name;
    std::string                         led_prefix;
    bool                                led_numbered;
} nzxt_hue2_zone;

typedef struct
{
    unsigned short                      pid;
    unsigned int                        num_rgb_channels;
    unsigned int                        num_fan_channels;
    unsigned int                        packet_size;
    nzxt_hue2_protocol                  protocol;
    const nzxt_hue2_zone*               zones[NZXT_HUE_2_MAX_CHANNELS];
} nzxt_hue2_device;

/*---------------------------------------------------------*\
| These constant values are defined in NZXTHue2Devices.cpp  |
\*---------------------------------------------------------*/
extern const unsigned int       NZXT_HUE_2_NUM_DEVICES;
extern const nzxt_hue2_device** nzxt_hue2_device_list;

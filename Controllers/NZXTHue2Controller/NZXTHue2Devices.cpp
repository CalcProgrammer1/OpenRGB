/*---------------------------------------------------------*\
| NZXTHue2Devices.cpp                                       |
|                                                           |
|   Device list for NZXT Hue 2 devices                      |
|                                                           |
|   Ferrah Aiko (ferrahwolfeh@proton.me)        12 Nov 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "NZXTHue2Devices.h"

/*---------------------------------------------------------*\
| ZONES                                                     |
\*---------------------------------------------------------*/
static const nzxt_hue2_zone hue_2_channel_1_zone =
{
    "Hue 2 Channel 1",
    "Hue 2 Channel 1, LED ",
    true
};

static const nzxt_hue2_zone hue_2_channel_2_zone =
{
    "Hue 2 Channel 2",
    "Hue 2 Channel 2, LED ",
    true
};

static const nzxt_hue2_zone hue_2_channel_3_zone =
{
    "Hue 2 Channel 3",
    "Hue 2 Channel 3, LED ",
    true
};

static const nzxt_hue2_zone hue_2_channel_4_zone =
{
    "Hue 2 Channel 4",
    "Hue 2 Channel 4, LED ",
    true
};

static const nzxt_hue2_zone hue_2_channel_5_zone =
{
    "Hue 2 Channel 5",
    "Hue 2 Channel 5, LED ",
    true
};

static const nzxt_hue2_zone hue_2_channel_6_zone =
{
    "Hue 2 Channel 6",
    "Hue 2 Channel 6, LED ",
    true
};

static const nzxt_hue2_zone kraken_x3_rgb_header_zone =
{
    "RGB Header",
    "RGB Header - LED ",
    true
};

static const nzxt_hue2_zone kraken_x3_ring_zone =
{
    "Ring",
    "Ring LED ",
    true
};

static const nzxt_hue2_zone kraken_x3_logo_zone =
{
    "Logo",
    "Logo LED",
    false
};

static const nzxt_hue2_zone kraken_2024_plus_fans_zone =
{
    "Fans",
    "Fan LED ",
    true
};

/*---------------------------------------------------------*\
| DEVICES                                                   |
\*---------------------------------------------------------*/
static const nzxt_hue2_device nzxt_hue_2_device =
{
    NZXT_HUE_2_PID,
    4,
    0,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        &hue_2_channel_3_zone,
        &hue_2_channel_4_zone,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_hue_2_ambient_device =
{
    NZXT_HUE_2_AMBIENT_PID,
    2,
    0,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_hue_2_motherboard_device =
{
    NZXT_MOTHERBOARD_DEVICE_PID,
    2,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_hue_2_motherboard_2_device =
{
    NZXT_MOTHERBOARD_DEVICE_2_PID,
    2,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_smart_device_v2_device =
{
    NZXT_SMART_DEVICE_V2_PID,
    2,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_smart_device_v2_1_device =
{
    NZXT_SMART_DEVICE_V2_1_PID,
    2,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_smart_device_v2_2_device =
{
    NZXT_SMART_DEVICE_V2_2_PID,
    2,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_kraken_x3_series_device =
{
    NZXT_KRAKEN_X3_SERIES_PID,
    3,
    0,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &kraken_x3_rgb_header_zone,
        &kraken_x3_ring_zone,
        &kraken_x3_logo_zone,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_kraken_x3_series_rgb_device =
{
    NZXT_KRAKEN_X3_SERIES_RGB_PID,
    3,
    0,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &kraken_x3_rgb_header_zone,
        &kraken_x3_ring_zone,
        &kraken_x3_logo_zone,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_kraken_2024_elite_series_rgb_device =
{
    NZXT_KRAKEN_2024_ELITE_SERIES_RGB_PID,
    2,
    2,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_2023,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_kraken_2024_plus_device =
{
    NZXT_KRAKEN_2024_PLUS_PID,
    1,
    0,
    NZXT_HUE_2_PACKET_SIZE_512,
    NZXT_HUE_2_PROTOCOL_2024_PLUS,
    {
        &kraken_2024_plus_fans_zone,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_rgb_fan_controller_device =
{
    NZXT_RGB_FAN_CONTROLLER_PID,
    2,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_rgb_fan_controller2_device =
{
    NZXT_RGB_FAN_CONTROLLER2_PID,
    2,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_rgb_fan_controller3_device =
{
    NZXT_RGB_FAN_CONTROLLER3_PID,
    2,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        NULL,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_rgb_fan_controller4_device =
{
    NZXT_RGB_FAN_CONTROLLER4_PID,
    6,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        &hue_2_channel_3_zone,
        &hue_2_channel_4_zone,
        &hue_2_channel_5_zone,
        &hue_2_channel_6_zone
    }
};

static const nzxt_hue2_device nzxt_rgb_fan_controller5_device =
{
    NZXT_RGB_FAN_CONTROLLER5_PID,
    6,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        &hue_2_channel_3_zone,
        &hue_2_channel_4_zone,
        &hue_2_channel_5_zone,
        &hue_2_channel_6_zone
    }
};

static const nzxt_hue2_device nzxt_rgb_fan_controller6_device =
{
    NZXT_RGB_FAN_CONTROLLER6_PID,
    6,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        &hue_2_channel_3_zone,
        &hue_2_channel_4_zone,
        &hue_2_channel_5_zone,
        &hue_2_channel_6_zone
    }
};

static const nzxt_hue2_device nzxt_rgb_fan_controller7_device =
{
    NZXT_RGB_FAN_CONTROLLER7_PID,
    6,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        &hue_2_channel_3_zone,
        &hue_2_channel_4_zone,
        &hue_2_channel_5_zone,
        &hue_2_channel_6_zone
    }
};

static const nzxt_hue2_device nzxt_rgb_fan_controller8_device =
{
    NZXT_RGB_FAN_CONTROLLER8_PID,
    6,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        &hue_2_channel_3_zone,
        &hue_2_channel_4_zone,
        &hue_2_channel_5_zone,
        &hue_2_channel_6_zone
    }
};

static const nzxt_hue2_device nzxt_rgb_fan_controller9_device =
{
    NZXT_RGB_FAN_CONTROLLER9_PID,
    6,
    3,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        &hue_2_channel_3_zone,
        &hue_2_channel_4_zone,
        &hue_2_channel_5_zone,
        &hue_2_channel_6_zone
    }
};

static const nzxt_hue2_device nzxt_rgb_controller_1_device =
{
    NZXT_RGB_CONTROLLER_1_PID,
    3,
    0,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        &hue_2_channel_3_zone,
        NULL,
        NULL,
        NULL
    }
};

static const nzxt_hue2_device nzxt_rgb_controller_2_device =
{
    NZXT_RGB_CONTROLLER_2_PID,
    3,
    0,
    NZXT_HUE_2_PACKET_SIZE_64,
    NZXT_HUE_2_PROTOCOL_LEGACY,
    {
        &hue_2_channel_1_zone,
        &hue_2_channel_2_zone,
        &hue_2_channel_3_zone,
        NULL,
        NULL,
        NULL
    }
};

/*---------------------------------------------------------*\
| DEVICE MASTER LIST                                        |
\*---------------------------------------------------------*/
static const nzxt_hue2_device* device_list[] =
{
    &nzxt_hue_2_device,
    &nzxt_hue_2_ambient_device,
    &nzxt_hue_2_motherboard_device,
    &nzxt_hue_2_motherboard_2_device,
    &nzxt_smart_device_v2_device,
    &nzxt_smart_device_v2_1_device,
    &nzxt_smart_device_v2_2_device,
    &nzxt_kraken_x3_series_device,
    &nzxt_kraken_x3_series_rgb_device,
    &nzxt_kraken_2024_elite_series_rgb_device,
    &nzxt_kraken_2024_plus_device,
    &nzxt_rgb_fan_controller_device,
    &nzxt_rgb_fan_controller2_device,
    &nzxt_rgb_fan_controller3_device,
    &nzxt_rgb_fan_controller4_device,
    &nzxt_rgb_fan_controller5_device,
    &nzxt_rgb_fan_controller6_device,
    &nzxt_rgb_fan_controller7_device,
    &nzxt_rgb_fan_controller8_device,
    &nzxt_rgb_fan_controller9_device,
    &nzxt_rgb_controller_1_device,
    &nzxt_rgb_controller_2_device
};

const unsigned int       NZXT_HUE_2_NUM_DEVICES = (sizeof(device_list) / sizeof(device_list[ 0 ]));
const nzxt_hue2_device** nzxt_hue2_device_list  = device_list;

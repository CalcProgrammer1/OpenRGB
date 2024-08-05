/*---------------------------------------------------------*\
| LianLiUniHubSLV2Controller.h                              |
|                                                           |
|   Driver for Lian Li SLV2 Uni Hub                         |
|                                                           |
|   Will Kennedy                                17 Jan 2023 |
|   Oliver P                                    26 Apr 2022 |
|   Credit to Luca Lovisa for original work.                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*----------------------------------------------------------------------------*\
| Global definitions.                                                          |
\*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*\
| Definitions related to zone Sizes                                            |
\*----------------------------------------------------------------------------*/


enum
{
    UNIHUB_SLV2_CHANNEL_COUNT              = 0x04,   /* Channel count                   */
    UNIHUB_SLV2_CHAN_LED_COUNT             = 0x10 * 6,   /* Max-LED per channel count - 96  */
};

/*----------------------------------------------------------------------------*\
| Definitions related to LED configuration.                                    |
\*----------------------------------------------------------------------------*/

// Used for sync'd mode between Fan and Edge

enum
{
    UNIHUB_SLV2_LED_MODE_STATIC_COLOR   = 0x01, // full data array
    UNIHUB_SLV2_LED_MODE_BREATHING      = 0x02, // full data array
    UNIHUB_SLV2_LED_MODE_RAINBOW_MORPH  = 0x04, // no array
    UNIHUB_SLV2_LED_MODE_RAINBOW        = 0x05, // no array
    UNIHUB_SLV2_LED_MODE_STAGGERED      = 0x18, // size 2
    UNIHUB_SLV2_LED_MODE_TIDE           = 0x1A, // size 2
    UNIHUB_SLV2_LED_MODE_RUNWAY         = 0x1C, // size 2
    UNIHUB_SLV2_LED_MODE_MIXING         = 0x1E, // size 2
    UNIHUB_SLV2_LED_MODE_STACK          = 0x20, // size 1
    UNIHUB_SLV2_LED_MODE_STACK_MULTI_COLOR = 0x21, // no array
    UNIHUB_SLV2_LED_MODE_NEON           = 0x22, // no array
    UNIHUB_SLV2_LED_MODE_COLOR_CYCLE    = 0x23, // size 3
    UNIHUB_SLV2_LED_MODE_METEOR         = 0x24, // size 2
    UNIHUB_SLV2_LED_MODE_VOICE          = 0x26, // no array
    UNIHUB_SLV2_LED_MODE_GROOVE         = 0x27, // size 2
    UNIHUB_SLV2_LED_MODE_RENDER         = 0x28, // size 4
    UNIHUB_SLV2_LED_MODE_TUNNEL         = 0x29, // size 4
    // merged modes
    UNIHUB_SLV2_LED_MODE_METEOR_MERGED  = 0x2A,
    UNIHUB_SLV2_LED_MODE_RUNWAY_MERGED  = 0x2B,
    UNIHUB_SLV2_LED_MODE_TIDE_MERGED    = 0x2C,
    UNIHUB_SLV2_LED_MODE_MIXING_MERGED  = 0x2D,
    UNIHUB_SLV2_LED_MODE_STACK_MULTI_COLOR_MERGED = 0x2E
};

enum
{
    UNIHUB_SLV2_LED_SPEED_000              = 0x02,   /* Very slow speed         */
    UNIHUB_SLV2_LED_SPEED_025              = 0x01,   /* Rather slow speed       */
    UNIHUB_SLV2_LED_SPEED_050              = 0x00,   /* Medium speed            */
    UNIHUB_SLV2_LED_SPEED_075              = 0xFF,   /* Rather fast speed       */
    UNIHUB_SLV2_LED_SPEED_100              = 0xFE,   /* Very fast speed         */
};

enum
{
    UNIHUB_SLV2_LED_DIRECTION_LTR          = 0x00,   /* Left-to-Right direction */
    UNIHUB_SLV2_LED_DIRECTION_RTL          = 0x01,   /* Right-to-Left direction */
};

enum
{
    UNIHUB_SLV2_LED_BRIGHTNESS_000         = 0x08,   /* Very dark (off)         */
    UNIHUB_SLV2_LED_BRIGHTNESS_025         = 0x03,   /* Rather dark             */
    UNIHUB_SLV2_LED_BRIGHTNESS_050         = 0x02,   /* Medium bright           */
    UNIHUB_SLV2_LED_BRIGHTNESS_075         = 0x01,   /* Rather bright           */
    UNIHUB_SLV2_LED_BRIGHTNESS_100         = 0x00,   /* Very bright             */
};

enum
{
    UNIHUB_SLV2_LED_LIMITER                = 0x01    /* Limit the color white to 999999 as per manufacturer limits */
};


/*----------------------------------------------------------------------------*\
| Definitions related to packet configuration.                                 |
\*----------------------------------------------------------------------------*/

enum
{
    UNIHUB_SLV2_TRANSACTION_ID            = 0xE0, /* Command value to start all packets */
};

/*----------------------------------------------------------------------------*\
| Uni Hub SLV2 controller.                                                       |
\*----------------------------------------------------------------------------*/

class LianLiUniHubSLV2Controller
{


public:
    LianLiUniHubSLV2Controller(hid_device* dev_handle, const char* path, std::string dev_name);
    ~LianLiUniHubSLV2Controller();

    std::string                 GetDeviceLocation();
    std::string                 GetFirmwareVersionString();
    std::string                 GetName();
    std::string                 GetSerialString();

    void                SetChannelMode
                                (
                                unsigned char   channel,
                                const mode active_mode,
                                unsigned int    num_fans
                                );

    void                SetChannelLEDs
                                (
                                unsigned char   channel,
                                RGBColor *      colors,
                                unsigned int    num_colors,
                                float           brightness
                                );

    void                SendStartAction
                                (
                                unsigned char   channel,
                                unsigned int    num_fans
                                );

    void                SendColorData
                                (
                                unsigned char   channel,            // Zone index
                                unsigned int    num_leds,
                                unsigned char*  led_data            // Color data payload
                                );

    void                SendCommitAction
                                (
                                unsigned char   channel,            // Zone index
                                unsigned char   effect,
                                unsigned char   speed,
                                unsigned int    direction,
                                unsigned int    brightness
                                );

private:
    /* The Uni Hub requires colors in RBG order */
    struct Color
    {
        uint8_t r;
        uint8_t b;
        uint8_t g;
    };

    /* The values correspond to the definitions above */
    struct Channel
    {
        uint8_t  index;

        uint8_t  anyFanCountOffset;
        uint8_t  anyFanCount;

        uint16_t ledActionAddress;
        uint16_t ledCommitAddress;
        uint16_t ledModeAddress;
        uint16_t ledSpeedAddress;
        uint16_t ledDirectionAddress;
        uint16_t ledBrightnessAddress;

        Color    colors[UNIHUB_SLV2_CHAN_LED_COUNT];

        uint8_t  ledMode;
        uint8_t  ledSpeed;
        uint8_t  ledDirection;
        uint8_t  ledBrightness;

        uint16_t fanHubActionAddress;
        uint16_t fanHubCommitAddress;

        uint16_t fanPwmActionAddress;
        uint16_t fanPwmCommitAddress;
        uint16_t fanRpmActionAddress;

        uint16_t fanSpeed;
    };

private:
    hid_device*             dev;

    /*---------------------------------------------------------*\
    | Device information strings                                |
    \*---------------------------------------------------------*/
    std::string             firmware_version;
    std::string             location;
    std::string             name;
};

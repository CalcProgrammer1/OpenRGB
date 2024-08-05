/*---------------------------------------------------------*\
| LianLiUniHubALController.h                                |
|                                                           |
|   Driver for Lian Li AL Uni Hub                           |
|                                                           |
|   Oliver P                                    26 Apr 2022 |
|   Credit to Luca Lovisa for original work                 |
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
    UNIHUB_AL_CHANNEL_COUNT              = 0x04,   /* Channel count                   */
    UNIHUB_AL_CHAN_FANLED_COUNT          = 0x20,   /* Max-LED per channel count - 32  */
    UNIHUB_AL_CHAN_EDGELED_COUNT         = 0x30,   /* Max-LED per channel count - 48  */
    UNIHUB_AL_CHAN_LED_COUNT             = 0x50,   /* Max-LED per channel count - 80  */
};

/*----------------------------------------------------------------------------*\
| Definitions related to LED configuration.                                    |
\*----------------------------------------------------------------------------*/

// Used for sync'd mode between Fan and Edge

enum
{
    UNIHUB_AL_LED_MODE_RAINBOW           = 0x28,   /* Rainbow mode           - Calls Fan only */
    UNIHUB_AL_LED_MODE_RAINBOW_MORPH     = 0x35,   /* Rainbow Morph mode     - Calls Fan only */
    UNIHUB_AL_LED_MODE_STATIC_COLOR      = 0x01,   /* Static Color mode       */
    UNIHUB_AL_LED_MODE_BREATHING         = 0x02,   /* Breathing mode          */
    UNIHUB_AL_LED_MODE_TAICHI            = 0x2C,   /* Neon mode              - Calls Fan only */
    UNIHUB_AL_LED_MODE_COLOR_CYCLE       = 0x2B,   /* Color Cycle mode       - Calls Fan only */
    UNIHUB_AL_LED_MODE_RUNWAY            = 0x1A,   /* Runway mode             */
    UNIHUB_AL_LED_MODE_METEOR            = 0x19,   /* Meteor mode             */
    UNIHUB_AL_LED_MODE_WARNING           = 0x2D,   /* Warning mode           - Calls Fan only */
    UNIHUB_AL_LED_MODE_VOICE             = 0x2E,   /* Voice mode             - Calls Fan only */
    UNIHUB_AL_LED_MODE_SPINNING_TEACUP   = 0x38,   /* Spinning Teacup mode   - Calls Fan only */
    UNIHUB_AL_LED_MODE_TORNADO           = 0x36,   /* Tornado mode           - Calls Fan only */
    UNIHUB_AL_LED_MODE_MIXING            = 0x2F,   /* Mixing mode            - Calls Fan only */
    UNIHUB_AL_LED_MODE_STACK             = 0x30,   /* Stack mode             - Calls Fan only */
    UNIHUB_AL_LED_MODE_STAGGGERED        = 0x37,   /* Stagggered mode        - Calls Fan only */
    UNIHUB_AL_LED_MODE_TIDE              = 0x31,   /* Tide mode              - Calls Fan only */
    UNIHUB_AL_LED_MODE_SCAN              = 0x32,   /* Scan mode              - Calls Fan only */
    UNIHUB_AL_LED_MODE_CONTEST           = 0x33,   /* Contest mode           - Calls Fan only */

};

enum
{
    UNIHUB_AL_LED_SPEED_000              = 0x02,   /* Very slow speed         */
    UNIHUB_AL_LED_SPEED_025              = 0x01,   /* Rather slow speed       */
    UNIHUB_AL_LED_SPEED_050              = 0x00,   /* Medium speed            */
    UNIHUB_AL_LED_SPEED_075              = 0xFF,   /* Rather fast speed       */
    UNIHUB_AL_LED_SPEED_100              = 0xFE,   /* Very fast speed         */
};

enum
{
    UNIHUB_AL_LED_DIRECTION_LTR          = 0x00,   /* Left-to-Right direction */
    UNIHUB_AL_LED_DIRECTION_RTL          = 0x01,   /* Right-to-Left direction */
};

enum
{
    UNIHUB_AL_LED_BRIGHTNESS_000         = 0x08,   /* Very dark (off)         */
    UNIHUB_AL_LED_BRIGHTNESS_025         = 0x03,   /* Rather dark             */
    UNIHUB_AL_LED_BRIGHTNESS_050         = 0x02,   /* Medium bright           */
    UNIHUB_AL_LED_BRIGHTNESS_075         = 0x01,   /* Rather bright           */
    UNIHUB_AL_LED_BRIGHTNESS_100         = 0x00,   /* Very bright             */
};

enum
{
    UNIHUB_AL_LED_LIMITER                = 0x01    /* Limit the color white to 999999 as per manufacturer limits */
};


/*----------------------------------------------------------------------------*\
| Definitions related to packet configuration.                                 |
\*----------------------------------------------------------------------------*/

enum
{
    UNIHUB_AL_TRANSACTION_ID            = 0xE0, /* Command value to start all packets */
};

/*----------------------------------------------------------------------------*\
| Uni Hub AL controller.                                                       |
\*----------------------------------------------------------------------------*/

class LianLiUniHubALController
{


public:
    LianLiUniHubALController(hid_device* dev_handle, const char* path, unsigned short pid, std::string dev_name);
    ~LianLiUniHubALController();

    std::string                 GetDeviceLocation();
    std::string                 GetFirmwareVersionString();
    std::string                 GetName();
    std::string                 GetSerialString();

    void                SetChannelMode
                                (
                                unsigned char   channel,
                                unsigned int    mode_value,
                                std::vector<RGBColor>        colors,    // Not a pointer because the copy gets resized
                                unsigned int    num_colors,
                                unsigned int    num_fans,
                                bool            upd_both_fan_edge,
                                unsigned int    brightness,
                                unsigned int    speed,
                                unsigned int    direction
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
                                unsigned int    fan_or_edge,        // 1 (Fan) or 0 (Edge) modifer to channel
                                unsigned int    num_leds,
                                unsigned char*  led_data            // Color data payload
                                );

    void                SendCommitAction
                                (
                                unsigned char   channel,            // Zone index
                                unsigned int    fan_or_edge,        // 1 (Fan) or 0 (Edge) modifer to channel
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

        Color    colors[UNIHUB_AL_CHAN_FANLED_COUNT];

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
    unsigned short          dev_pid;

    /*---------------------------------------------------------*\
    | Device information strings                                |
    \*---------------------------------------------------------*/
    std::string             firmware_version;
    std::string             location;
    std::string             name;
};

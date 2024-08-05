/*---------------------------------------------------------*\
| LianLiUniHub_AL10Controller.h                             |
|                                                           |
|   Driver for Lian Li AL10 Uni Hub                         |
|                                                           |
|   Oliver P                                    05 May 2022 |
|   Credit to Luca Lovisa for original work                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstdint>
#include <mutex>
#include <string>
#include <libusb.h>
#include "RGBController.h"

/*----------------------------------------------------------------------------*\
| Global definitions.                                                          |
\*----------------------------------------------------------------------------*/

enum
{
    UNIHUB_AL10_CHANNEL_COUNT              = 0x04,   /* Channel count              */
    UNIHUB_AL10_CHANLED_COUNT              = 0x50,   /* Max-LED per channel count  */
};

enum
{
    UNIHUB_AL10_ACTION_ADDRESS             = 0xE020, /* Global action address      */
    UNIHUB_AL10_COMMIT_ADDRESS             = 0xE02F, /* Global commit address      */
};

enum
{
    UNIHUB_AL10_ANY_C1_FAN_COUNT_OFFSET    = 0x00,   /* Channel 1 fan count offset */
    UNIHUB_AL10_ANY_C2_FAN_COUNT_OFFSET    = 0x14,   /* Channel 2 fan count offset */
    UNIHUB_AL10_ANY_C3_FAN_COUNT_OFFSET    = 0x28,   /* Channel 3 fan count offset */
    UNIHUB_AL10_ANY_C4_FAN_COUNT_OFFSET    = 0x3C,   /* Channel 4 fan count offset */
};

enum
{
    UNIHUB_AL10_ANY_FAN_COUNT_000          = 0xFF,   /* Fan count for 0 fans (dummy value) */
    UNIHUB_AL10_ANY_FAN_COUNT_001          = 0x00,   /* Fan count for 1 fan        */
    UNIHUB_AL10_ANY_FAN_COUNT_002          = 0x01,   /* Fan count for 2 fans       */
    UNIHUB_AL10_ANY_FAN_COUNT_003          = 0x02,   /* Fan count for 3 fans       */
    UNIHUB_AL10_ANY_FAN_COUNT_004          = 0x03,   /* Fan count for 4 fans       */
};

enum
{
    UNIHUB_AL10_LED_LIMITER                = 1,      /* Limit the color white to 999999 as per manufacturer limits in v1.7 */
};

/*----------------------------------------------------------------------------*\
| Definitions related to led configuration.                                    |
\*----------------------------------------------------------------------------*/

enum
{
    UNIHUB_AL10_LED_C1_ACTION_ADDRESS      = 0xE500, /* Channel 1 led action address     */
    UNIHUB_AL10_LED_C1_COMMIT_ADDRESS      = 0xE02F, /* Channel 1 led commit address     */
    UNIHUB_AL10_LED_C1_MODE_ADDRESS        = 0xE021, /* Channel 1 led mode address       */
    UNIHUB_AL10_LED_C1_SPEED_ADDRESS       = 0xE022, /* Channel 1 led speed address      */
    UNIHUB_AL10_LED_C1_DIRECTION_ADDRESS   = 0xE023, /* Channel 1 led direction address  */
    UNIHUB_AL10_LED_C1_BRIGHTNESS_ADDRESS  = 0xE029, /* Channel 1 led brightness address */

    UNIHUB_AL10_LED_C2_ACTION_ADDRESS      = 0xE5F0, /* Channel 2 led action address     */
    UNIHUB_AL10_LED_C2_COMMIT_ADDRESS      = 0xE03F, /* Channel 2 led commit address     */
    UNIHUB_AL10_LED_C2_MODE_ADDRESS        = 0xE031, /* Channel 2 led mode address       */
    UNIHUB_AL10_LED_C2_SPEED_ADDRESS       = 0xE032, /* Channel 2 led speed address      */
    UNIHUB_AL10_LED_C2_DIRECTION_ADDRESS   = 0xE033, /* Channel 2 led direction address  */
    UNIHUB_AL10_LED_C2_BRIGHTNESS_ADDRESS  = 0xE039, /* Channel 2 led brightness address */

    UNIHUB_AL10_LED_C3_ACTION_ADDRESS      = 0xE6E0, /* Channel 3 led action address     */
    UNIHUB_AL10_LED_C3_COMMIT_ADDRESS      = 0xE04F, /* Channel 3 led commit address     */
    UNIHUB_AL10_LED_C3_MODE_ADDRESS        = 0xE041, /* Channel 3 led mode address       */
    UNIHUB_AL10_LED_C3_SPEED_ADDRESS       = 0xE042, /* Channel 3 led speed address      */
    UNIHUB_AL10_LED_C3_DIRECTION_ADDRESS   = 0xE043, /* Channel 3 led direction address  */
    UNIHUB_AL10_LED_C3_BRIGHTNESS_ADDRESS  = 0xE049, /* Channel 3 led brightness address */

    UNIHUB_AL10_LED_C4_ACTION_ADDRESS      = 0xE7D0, /* Channel 4 led action address     */
    UNIHUB_AL10_LED_C4_COMMIT_ADDRESS      = 0xE05F, /* Channel 4 led commit address     */
    UNIHUB_AL10_LED_C4_MODE_ADDRESS        = 0xE051, /* Channel 4 led mode address       */
    UNIHUB_AL10_LED_C4_SPEED_ADDRESS       = 0xE052, /* Channel 4 led speed address      */
    UNIHUB_AL10_LED_C4_DIRECTION_ADDRESS   = 0xE053, /* Channel 4 led direction address  */
    UNIHUB_AL10_LED_C4_BRIGHTNESS_ADDRESS  = 0xE059, /* Channel 4 led brightness address */
};

enum
{
    UNIHUB_AL10_LED_MODE_RAINBOW           = 0x05,   /* Rainbow mode            */
    UNIHUB_AL10_LED_MODE_RAINBOW_MORPH     = 0xFF,   /* Runway mode            - Needs updated code */
    UNIHUB_AL10_LED_MODE_STATIC_COLOR      = 0x01,   /* Static Color mode       */
    UNIHUB_AL10_LED_MODE_BREATHING         = 0x02,   /* Breathing mode          */
    UNIHUB_AL10_LED_MODE_TAICHI            = 0x2C,   /* Neon mode               */
    UNIHUB_AL10_LED_MODE_COLOR_CYCLE       = 0x2B,   /* Color Cycle mode        */
    UNIHUB_AL10_LED_MODE_RUNWAY            = 0xFF,   /* Runway mode            - Needs updated code */
    UNIHUB_AL10_LED_MODE_METEOR            = 0xFF,   /* Meteor mode            - Needs updated code */
    UNIHUB_AL10_LED_MODE_WARNING           = 0x2D,   /* Warning mode            */
    UNIHUB_AL10_LED_MODE_VOICE             = 0x22,   /* Voice mode              */
    UNIHUB_AL10_LED_MODE_SPINNING_TEACUP   = 0x36,   /* Spinning Teacup mode    */
    UNIHUB_AL10_LED_MODE_TORNADO           = 0x34,   /* Tornado mode            */
    UNIHUB_AL10_LED_MODE_MIXING            = 0x23,   /* Mixing mode             */
    UNIHUB_AL10_LED_MODE_STACK             = 0xFF,   /* Stack mode             - Needs updated code */
    UNIHUB_AL10_LED_MODE_STAGGGERED        = 0x35,   /* Stagggered mode         */
    UNIHUB_AL10_LED_MODE_TIDE              = 0x25,   /* Tide mode               */
    UNIHUB_AL10_LED_MODE_SCAN              = 0x26,   /* Scan mode               */
    UNIHUB_AL10_LED_MODE_CONTEST           = 0x33,   /* Contest mode            */
};

enum
{
    UNIHUB_AL10_LED_SPEED_000              = 0x02,   /* Very slow speed         */
    UNIHUB_AL10_LED_SPEED_025              = 0x01,   /* Rather slow speed       */
    UNIHUB_AL10_LED_SPEED_050              = 0x00,   /* Medium speed            */
    UNIHUB_AL10_LED_SPEED_075              = 0xFF,   /* Rather fast speed       */
    UNIHUB_AL10_LED_SPEED_100              = 0xFE,   /* Very fast speed         */
};

enum
{
    UNIHUB_AL10_LED_DIRECTION_LTR          = 0x00,   /* Left-to-Right direction */
    UNIHUB_AL10_LED_DIRECTION_RTL          = 0x01,   /* Right-to-Left direction */
};

enum
{
    UNIHUB_AL10_LED_BRIGHTNESS_000         = 0x08,   /* Very dark (off)         */
    UNIHUB_AL10_LED_BRIGHTNESS_025         = 0x03,   /* Rather dark             */
    UNIHUB_AL10_LED_BRIGHTNESS_050         = 0x02,   /* Medium bright           */
    UNIHUB_AL10_LED_BRIGHTNESS_075         = 0x01,   /* Rather bright           */
    UNIHUB_AL10_LED_BRIGHTNESS_100         = 0x00,   /* Very bright             */
};

/*----------------------------------------------------------------------------*\
| Definitions related to fan configuration.                                    |
\*----------------------------------------------------------------------------*/

enum
{
    UNIHUB_AL10_FAN_C1_HUB_ACTION_ADDRESS  = 0xE8A0, /* Channel 1 fan action address for hub control */
    UNIHUB_AL10_FAN_C1_HUB_COMMIT_ADDRESS  = 0xE890, /* Channel 1 fan commit address for hub control */
    UNIHUB_AL10_FAN_C1_PWM_ACTION_ADDRESS  = 0xE890, /* Channel 1 fan action address for pwm control */
    UNIHUB_AL10_FAN_C1_PWM_COMMIT_ADDRESS  = 0xE818, /* Channel 1 fan commit address for pwm control */
    UNIHUB_AL10_FAN_C1_RPM_ACTION_ADDRESS  = 0xE800, /* Channel 1 fan pwm read address               */

    UNIHUB_AL10_FAN_C2_HUB_ACTION_ADDRESS  = 0xE8A2, /* Channel 2 fan action address for hub control */
    UNIHUB_AL10_FAN_C2_HUB_COMMIT_ADDRESS  = 0xE891, /* Channel 2 fan commit address for hub control */
    UNIHUB_AL10_FAN_C2_PWM_ACTION_ADDRESS  = 0xE891, /* Channel 2 fan action address for pwm control */
    UNIHUB_AL10_FAN_C2_PWM_COMMIT_ADDRESS  = 0xE81A, /* Channel 2 fan commit address for pwm control */
    UNIHUB_AL10_FAN_C2_RPM_ACTION_ADDRESS  = 0xE802, /* Channel 1 fan pwm read address               */

    UNIHUB_AL10_FAN_C3_HUB_ACTION_ADDRESS  = 0xE8A4, /* Channel 3 fan action address for hub control */
    UNIHUB_AL10_FAN_C3_HUB_COMMIT_ADDRESS  = 0xE892, /* Channel 3 fan commit address for hub control */
    UNIHUB_AL10_FAN_C3_PWM_ACTION_ADDRESS  = 0xE892, /* Channel 3 fan action address for pwm control */
    UNIHUB_AL10_FAN_C3_PWM_COMMIT_ADDRESS  = 0xE81C, /* Channel 3 fan commit address for pwm control */
    UNIHUB_AL10_FAN_C3_RPM_ACTION_ADDRESS  = 0xE804, /* Channel 1 fan pwm read address               */

    UNIHUB_AL10_FAN_C4_HUB_ACTION_ADDRESS  = 0xE8A6, /* Channel 4 fan action address for hub control */
    UNIHUB_AL10_FAN_C4_HUB_COMMIT_ADDRESS  = 0xE893, /* Channel 4 fan commit address for hub control */
    UNIHUB_AL10_FAN_C4_PWM_ACTION_ADDRESS  = 0xE893, /* Channel 4 fan action address for pwm control */
    UNIHUB_AL10_FAN_C4_PWM_COMMIT_ADDRESS  = 0xE81E, /* Channel 4 fan commit address for pwm control */
    UNIHUB_AL10_FAN_C4_RPM_ACTION_ADDRESS  = 0xE806, /* Channel 1 fan pwm read address               */
};

enum
{
    UNIHUB_AL10_FAN_SPEED_QUIET            = 0x2003, /* Rather slow */
    UNIHUB_AL10_FAN_SPEED_HIGH_SPEED       = 0x2206, /* Rather fast */
    UNIHUB_AL10_FAN_SPEED_FULL_SPEED       = 0x6C07, /* BRRRRRRRRRR */
    UNIHUB_AL10_FAN_SPEED_PWM              = 0xFFFF, /* PWM Control */
};

/*----------------------------------------------------------------------------*\
| Uni Hub controller.                                                          |
\*----------------------------------------------------------------------------*/

class LianLiUniHub_AL10Controller
{
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

        Color    colors[UNIHUB_AL10_CHANLED_COUNT];

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

public:
    LianLiUniHub_AL10Controller
        (
        libusb_device*              device,
        libusb_device_descriptor*   descriptor
        );
    ~LianLiUniHub_AL10Controller();

    std::string GetVersion();
    std::string GetLocation();
    std::string GetSerial();

    void        SetAnyFanCount(size_t channel, uint8_t count);
    void        SetLedColors(size_t channel, RGBColor* colors, size_t count);
    void        SetLedMode(size_t channel, uint8_t mode);
    void        SetLedSpeed(size_t channel, uint8_t speed);
    void        SetLedDirection(size_t channel, uint8_t direction);
    void        SetLedBrightness(size_t channel, uint8_t brightness);
    uint16_t    GetFanSpeed(size_t channel);
    void        SetFanSpeed(size_t channel, uint16_t speed);
    void        EnableRgbhMode();
    void        DisableRgbhMode();
    void        EnableSyncMode();
    void        DisableSyncMode();
    uint16_t    ReadFanSpeed(size_t channel);

    /*-----------------------------------------------------*\
    | Synchronize the current configuration to the Uni Hub. |
    \*-----------------------------------------------------*/
    void        Synchronize();

private:
    libusb_device_handle* handle = nullptr;

    std::string version;
    std::string location;
    std::string serial;

    bool rgbhModeEnabled = false;
    bool syncModeEnabled = false;

    Channel channels[UNIHUB_AL10_CHANNEL_COUNT];

    void        CloseLibusb();
    std::string ReadVersion();
    void        SendConfig(uint16_t wIndex, uint8_t  *config, size_t length);
    void        SendCommit(uint16_t wIndex);
};

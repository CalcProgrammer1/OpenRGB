/*---------------------------------------------------------*\
| LianLiUniHubController.h                                  |
|                                                           |
|   Driver for Lian Li Uni Hub                              |
|                                                           |
|   Luca Lovisa                                 20 Feb 2021 |
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
    UNIHUB_CHANNEL_COUNT              = 0x04,   /* Channel count              */
    UNIHUB_CHANLED_COUNT              = 0x40,   /* Max-LED per channel count  */
};

enum
{
    UNIHUB_ACTION_ADDRESS             = 0xe021, /* Global action address      */
    UNIHUB_COMMIT_ADDRESS             = 0xe02f, /* Global commit address      */
};

enum
{
    UNIHUB_ANY_C1_FAN_COUNT_OFFSET    = 0x00,   /* Channel 1 fan count offset */
    UNIHUB_ANY_C2_FAN_COUNT_OFFSET    = 0x10,   /* Channel 2 fan count offset */
    UNIHUB_ANY_C3_FAN_COUNT_OFFSET    = 0x20,   /* Channel 3 fan count offset */
    UNIHUB_ANY_C4_FAN_COUNT_OFFSET    = 0x30,   /* Channel 4 fan count offset */
};

enum
{
    UNIHUB_ANY_FAN_COUNT_000          = 0xFF,   /* Fan count for 0 fans (dummy value) */
    UNIHUB_ANY_FAN_COUNT_001          = 0x00,   /* Fan count for 1 fan        */
    UNIHUB_ANY_FAN_COUNT_002          = 0x01,   /* Fan count for 2 fans       */
    UNIHUB_ANY_FAN_COUNT_003          = 0x02,   /* Fan count for 3 fans       */
    UNIHUB_ANY_FAN_COUNT_004          = 0x03,   /* Fan count for 4 fans       */
};

/*----------------------------------------------------------------------------*\
| Definitions related to led configuration.                                    |
\*----------------------------------------------------------------------------*/

enum
{
    UNIHUB_LED_C1_ACTION_ADDRESS      = 0xe300, /* Channel 1 led action address     */
    UNIHUB_LED_C1_COMMIT_ADDRESS      = 0xe02f, /* Channel 1 led commit address     */
    UNIHUB_LED_C1_MODE_ADDRESS        = 0xe021, /* Channel 1 led mode address       */
    UNIHUB_LED_C1_SPEED_ADDRESS       = 0xe022, /* Channel 1 led speed address      */
    UNIHUB_LED_C1_DIRECTION_ADDRESS   = 0xe023, /* Channel 1 led direction address  */
    UNIHUB_LED_C1_BRIGHTNESS_ADDRESS  = 0xe029, /* Channel 1 led brightness address */

    UNIHUB_LED_C2_ACTION_ADDRESS      = 0xe3c0, /* Channel 2 led action address     */
    UNIHUB_LED_C2_COMMIT_ADDRESS      = 0xe03f, /* Channel 2 led commit address     */
    UNIHUB_LED_C2_MODE_ADDRESS        = 0xe031, /* Channel 2 led mode address       */
    UNIHUB_LED_C2_SPEED_ADDRESS       = 0xe032, /* Channel 2 led speed address      */
    UNIHUB_LED_C2_DIRECTION_ADDRESS   = 0xe033, /* Channel 2 led direction address  */
    UNIHUB_LED_C2_BRIGHTNESS_ADDRESS  = 0xe039, /* Channel 2 led brightness address */

    UNIHUB_LED_C3_ACTION_ADDRESS      = 0xe480, /* Channel 3 led action address     */
    UNIHUB_LED_C3_COMMIT_ADDRESS      = 0xe04f, /* Channel 3 led commit address     */
    UNIHUB_LED_C3_MODE_ADDRESS        = 0xe041, /* Channel 3 led mode address       */
    UNIHUB_LED_C3_SPEED_ADDRESS       = 0xe042, /* Channel 3 led speed address      */
    UNIHUB_LED_C3_DIRECTION_ADDRESS   = 0xe043, /* Channel 3 led direction address  */
    UNIHUB_LED_C3_BRIGHTNESS_ADDRESS  = 0xe049, /* Channel 3 led brightness address */

    UNIHUB_LED_C4_ACTION_ADDRESS      = 0xe540, /* Channel 4 led action address     */
    UNIHUB_LED_C4_COMMIT_ADDRESS      = 0xe05f, /* Channel 4 led commit address     */
    UNIHUB_LED_C4_MODE_ADDRESS        = 0xe051, /* Channel 4 led mode address       */
    UNIHUB_LED_C4_SPEED_ADDRESS       = 0xe052, /* Channel 4 led speed address      */
    UNIHUB_LED_C4_DIRECTION_ADDRESS   = 0xe053, /* Channel 4 led direction address  */
    UNIHUB_LED_C4_BRIGHTNESS_ADDRESS  = 0xe059, /* Channel 4 led brightness address */
};

enum
{
    UNIHUB_LED_MODE_RAINBOW           = 0x05,   /* Rainbow mode            */
    UNIHUB_LED_MODE_STATIC_COLOR      = 0x01,   /* Static Color mode       */
    UNIHUB_LED_MODE_BREATHING         = 0x02,   /* Breathing mode          */
    UNIHUB_LED_MODE_COLOR_CYCLE       = 0x04,   /* Color Cycle mode        */
    UNIHUB_LED_MODE_RUNWAY            = 0x1c,   /* Runway mode             */
    UNIHUB_LED_MODE_RUNWAY_SYNC       = 0x1c,   /* Runway Sync mode        */
    UNIHUB_LED_MODE_STAGGGERED        = 0x18,   /* Stagggered mode         */
    UNIHUB_LED_MODE_MIXING            = 0x1a,   /* Mixing mode             */
    UNIHUB_LED_MODE_METEOR            = 0x07,   /* Meteor mode             */
    UNIHUB_LED_MODE_METEOR_SYNC       = 0x07,   /* Meteor Sync mode        */
    UNIHUB_LED_MODE_FIREWORK          = 0x1f,   /* Firework mode           */
    UNIHUB_LED_MODE_STACK             = 0x21,   /* Stack mode              */
    UNIHUB_LED_MODE_STACK_MULTI_COLOR = 0x22,   /* Stack Multi Color mode  */
    UNIHUB_LED_MODE_NEON              = 0x23,   /* Neon mode               */
};

enum
{
    UNIHUB_LED_SPEED_000              = 0x04,   /* Very slow speed         */
    UNIHUB_LED_SPEED_025              = 0x03,   /* Rather slow speed       */
    UNIHUB_LED_SPEED_050              = 0x02,   /* Medium speed            */
    UNIHUB_LED_SPEED_075              = 0x01,   /* Rather fast speed       */
    UNIHUB_LED_SPEED_100              = 0x00,   /* Very fast speed         */
};

enum
{
    UNIHUB_LED_DIRECTION_LTR          = 0x00,   /* Left-to-Right direction */
    UNIHUB_LED_DIRECTION_RTL          = 0x01,   /* Right-to-Left direction */
};

enum
{
    UNIHUB_LED_BRIGHTNESS_000         = 0x08,   /* Very dark (off)         */
    UNIHUB_LED_BRIGHTNESS_025         = 0x03,   /* Rather dark             */
    UNIHUB_LED_BRIGHTNESS_050         = 0x02,   /* Medium bright           */
    UNIHUB_LED_BRIGHTNESS_075         = 0x01,   /* Rather bright           */
    UNIHUB_LED_BRIGHTNESS_100         = 0x00,   /* Very bright             */
};

/*----------------------------------------------------------------------------*\
| Definitions related to fan configuration.                                    |
\*----------------------------------------------------------------------------*/

enum
{
    UNIHUB_FAN_C1_HUB_ACTION_ADDRESS  = 0xe8a0, /* Channel 1 fan action address for hub control */
    UNIHUB_FAN_C1_HUB_COMMIT_ADDRESS  = 0xe890, /* Channel 1 fan commit address for hub control */
    UNIHUB_FAN_C1_PWM_ACTION_ADDRESS  = 0xe890, /* Channel 1 fan action address for pwm control */
    UNIHUB_FAN_C1_PWM_COMMIT_ADDRESS  = 0xe818, /* Channel 1 fan commit address for pwm control */
    UNIHUB_FAN_C1_RPM_ACTION_ADDRESS  = 0xe800, /* Channel 1 fan pwm read address               */

    UNIHUB_FAN_C2_HUB_ACTION_ADDRESS  = 0xe8a2, /* Channel 2 fan action address for hub control */
    UNIHUB_FAN_C2_HUB_COMMIT_ADDRESS  = 0xe891, /* Channel 2 fan commit address for hub control */
    UNIHUB_FAN_C2_PWM_ACTION_ADDRESS  = 0xe891, /* Channel 2 fan action address for pwm control */
    UNIHUB_FAN_C2_PWM_COMMIT_ADDRESS  = 0xe81a, /* Channel 2 fan commit address for pwm control */
    UNIHUB_FAN_C2_RPM_ACTION_ADDRESS  = 0xe802, /* Channel 1 fan pwm read address               */

    UNIHUB_FAN_C3_HUB_ACTION_ADDRESS  = 0xe8a4, /* Channel 3 fan action address for hub control */
    UNIHUB_FAN_C3_HUB_COMMIT_ADDRESS  = 0xe892, /* Channel 3 fan commit address for hub control */
    UNIHUB_FAN_C3_PWM_ACTION_ADDRESS  = 0xe892, /* Channel 3 fan action address for pwm control */
    UNIHUB_FAN_C3_PWM_COMMIT_ADDRESS  = 0xe81c, /* Channel 3 fan commit address for pwm control */
    UNIHUB_FAN_C3_RPM_ACTION_ADDRESS  = 0xe804, /* Channel 1 fan pwm read address               */

    UNIHUB_FAN_C4_HUB_ACTION_ADDRESS  = 0xe8a6, /* Channel 4 fan action address for hub control */
    UNIHUB_FAN_C4_HUB_COMMIT_ADDRESS  = 0xe893, /* Channel 4 fan commit address for hub control */
    UNIHUB_FAN_C4_PWM_ACTION_ADDRESS  = 0xe893, /* Channel 4 fan action address for pwm control */
    UNIHUB_FAN_C4_PWM_COMMIT_ADDRESS  = 0xe81e, /* Channel 4 fan commit address for pwm control */
    UNIHUB_FAN_C4_RPM_ACTION_ADDRESS  = 0xe806, /* Channel 1 fan pwm read address               */
};

enum
{
    UNIHUB_FAN_SPEED_QUIET            = 0x2003, /* Rather slow */
    UNIHUB_FAN_SPEED_HIGH_SPEED       = 0x2206, /* Rather fast */
    UNIHUB_FAN_SPEED_FULL_SPEED       = 0x6c07, /* BRRRRRRRRRR */
    UNIHUB_FAN_SPEED_PWM              = 0xffff, /* PWM Control */
};

/*----------------------------------------------------------------------------*\
| Uni Hub controller.                                                          |
\*----------------------------------------------------------------------------*/

class LianLiUniHubController
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

        Color    colors[UNIHUB_CHANLED_COUNT];

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
    LianLiUniHubController
        (
        libusb_device*              device,
        libusb_device_descriptor*   descriptor
        );
    ~LianLiUniHubController();

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

    Channel channels[UNIHUB_CHANNEL_COUNT];

    void        CloseLibusb();
    std::string ReadVersion();
    void        SendConfig(uint16_t wIndex, uint8_t  *config, size_t length);
    void        SendCommit(uint16_t wIndex);
};

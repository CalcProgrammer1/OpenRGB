/*---------------------------------------------------------*\
| ZalmanZSyncController.h                                   |
|                                                           |
|   Driver for Zalman Z Sync                                |
|                                                           |
|   Based on CorsairLightingNodeConroller, the protocol is  |
|   the same as the Corsair Lighting Node except with 8     |
|   channels                                                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    ZALMAN_Z_SYNC_PACKET_ID_FIRMWARE                 = 0x02,     /* Get firmware version                 */
    ZALMAN_Z_SYNC_PACKET_ID_DIRECT                   = 0x32,     /* Direct mode LED update packet        */
    ZALMAN_Z_SYNC_PACKET_ID_COMMIT                   = 0x33,     /* Commit changes packet                */
    ZALMAN_Z_SYNC_PACKET_ID_BEGIN                    = 0x34,     /* Begin effect packet                  */
    ZALMAN_Z_SYNC_PACKET_ID_EFFECT_CONFIG            = 0x35,     /* Effect mode configuration packet     */
    ZALMAN_Z_SYNC_PACKET_ID_TEMPERATURE              = 0x36,     /* Update temperature value packet      */
    ZALMAN_Z_SYNC_PACKET_ID_RESET                    = 0x37,     /* Reset channel packet                 */
    ZALMAN_Z_SYNC_PACKET_ID_PORT_STATE               = 0x38,     /* Set port state packet                */
    ZALMAN_Z_SYNC_PACKET_ID_BRIGHTNESS               = 0x39,     /* Set brightness packet                */
    ZALMAN_Z_SYNC_PACKET_ID_LED_COUNT                = 0x3A,     /* Set LED count packet                 */
    ZALMAN_Z_SYNC_PACKET_ID_PROTOCOL                 = 0x3B,     /* Set protocol packet                  */
};

enum
{
    ZALMAN_Z_SYNC_DIRECT_CHANNEL_RED                 = 0x00,     /* Red channel for direct update        */
    ZALMAN_Z_SYNC_DIRECT_CHANNEL_GREEN               = 0x01,     /* Green channel for direct update      */
    ZALMAN_Z_SYNC_DIRECT_CHANNEL_BLUE                = 0x02,     /* Blue channel for direct update       */
};

enum
{
    ZALMAN_Z_SYNC_PORT_STATE_HARDWARE                = 0x01,     /* Effect hardware control of channel   */
    ZALMAN_Z_SYNC_PORT_STATE_SOFTWARE                = 0x02,     /* Direct software control of channel   */
};

enum
{
    ZALMAN_Z_SYNC_LED_TYPE_LED_STRIP                 = 0x0A,     /* Corsair LED Strip Type               */
    ZALMAN_Z_SYNC_LED_TYPE_HD_FAN                    = 0x0C,     /* Corsair HD-series Fan Type           */
    ZALMAN_Z_SYNC_LED_TYPE_SP_FAN                    = 0x01,     /* Corsair SP-series Fan Type           */
    ZALMAN_Z_SYNC_LED_TYPE_ML_FAN                    = 0x02,     /* Corsair ML-series Fan Type           */
};

enum
{
    ZALMAN_Z_SYNC_CHANNEL_1                          = 0x00,     /* Channel 1                            */
    ZALMAN_Z_SYNC_CHANNEL_2                          = 0x01,     /* Channel 2                            */
    ZALMAN_Z_SYNC_CHANNEL_3                          = 0x02,     /* Channel 3                            */
    ZALMAN_Z_SYNC_CHANNEL_4                          = 0x03,     /* Channel 4                            */
    ZALMAN_Z_SYNC_CHANNEL_5                          = 0x04,     /* Channel 5                            */
    ZALMAN_Z_SYNC_CHANNEL_6                          = 0x05,     /* Channel 6                            */
    ZALMAN_Z_SYNC_CHANNEL_7                          = 0x06,     /* Channel 7                            */
    ZALMAN_Z_SYNC_CHANNEL_8                          = 0x07,     /* Channel 8                            */
    ZALMAN_Z_SYNC_NUM_CHANNELS                       = 0x08,     /* Number of channels                   */
};

enum
{
    ZALMAN_Z_SYNC_SPEED_FAST                         = 0x00,     /* Fast speed                           */
    ZALMAN_Z_SYNC_SPEED_MEDIUM                       = 0x01,     /* Medium speed                         */
    ZALMAN_Z_SYNC_SPEED_SLOW                         = 0x02,     /* Slow speed                           */
};

enum
{
    ZALMAN_Z_SYNC_MODE_RAINBOW_WAVE                  = 0x00,     /* Rainbow Wave mode                    */
    ZALMAN_Z_SYNC_MODE_COLOR_SHIFT                   = 0x01,     /* Color Shift mode                     */
    ZALMAN_Z_SYNC_MODE_COLOR_PULSE                   = 0x02,     /* Color Pulse mode                     */
    ZALMAN_Z_SYNC_MODE_COLOR_WAVE                    = 0x03,     /* Color Wave mode                      */
    ZALMAN_Z_SYNC_MODE_STATIC                        = 0x04,     /* Static mode                          */
    ZALMAN_Z_SYNC_MODE_TEMPERATURE                   = 0x05,     /* Temperature mode                     */
    ZALMAN_Z_SYNC_MODE_VISOR                         = 0x06,     /* Visor mode                           */
    ZALMAN_Z_SYNC_MODE_MARQUEE                       = 0x07,     /* Marquee mode                         */
    ZALMAN_Z_SYNC_MODE_BLINK                         = 0x08,     /* Blink mode                           */
    ZALMAN_Z_SYNC_MODE_SEQUENTIAL                    = 0x09,     /* Sequential mode                      */
    ZALMAN_Z_SYNC_MODE_RAINBOW                       = 0x0A,     /* Rainbow mode                         */
};

class ZalmanZSyncController
{
public:
    ZalmanZSyncController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~ZalmanZSyncController();

    std::string     GetFirmwareString();
    std::string     GetLocationString();
    std::string     GetNameString();
    std::string     GetSerialString();

    unsigned int    GetStripsOnChannel(unsigned int channel);

    void            SetChannelEffect(unsigned char channel,
                                     unsigned char num_leds,
                                     unsigned char mode,
                                     unsigned char speed,
                                     unsigned char direction,
                                     bool          random,
                                     unsigned char red1,
                                     unsigned char grn1,
                                     unsigned char blu1,
                                     unsigned char red2,
                                     unsigned char grn2,
                                     unsigned char blu2,
                                     unsigned char red3,
                                     unsigned char grn3,
                                     unsigned char blu3
                                    );

    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);

    void            KeepaliveThread();

private:
    hid_device*             dev;
    std::string             firmware_version;
    std::string             location;
    std::string             name;
    std::thread*            keepalive_thread;
    std::atomic<bool>       keepalive_thread_run;
    std::chrono::time_point<std::chrono::steady_clock> last_commit_time;

    void            SendFirmwareRequest();

    void            SendDirect
                        (
                        unsigned char   channel,
                        unsigned char   start,
                        unsigned char   count,
                        unsigned char   color_channel,
                        unsigned char*  color_data
                        );

    void            SendCommit();

    void            SendBegin
                        (
                        unsigned char   channel
                        );

    void            SendEffectConfig
                        (
                        unsigned char   channel,
                        unsigned char   count,
                        unsigned char   led_type,
                        unsigned char   mode,
                        unsigned char   speed,
                        unsigned char   direction,
                        unsigned char   change_style,
                        unsigned char   color_0_red,
                        unsigned char   color_0_green,
                        unsigned char   color_0_blue,
                        unsigned char   color_1_red,
                        unsigned char   color_1_green,
                        unsigned char   color_1_blue,
                        unsigned char   color_2_red,
                        unsigned char   color_2_green,
                        unsigned char   color_2_blue,
                        unsigned short  temperature_0,
                        unsigned short  temperature_1,
                        unsigned short  temperature_2
                        );

    void            SendTemperature();

    void            SendReset
                        (
                        unsigned char   channel
                        );

    void            SendPortState
                        (
                        unsigned char   channel,
                        unsigned char   state
                        );

    void            SendBrightness();

    void            SendLEDCount();

    void            SendProtocol();
};

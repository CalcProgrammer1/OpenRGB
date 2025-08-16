/*---------------------------------------------------------*\
| LogitechG915Controller.h                                  |
|                                                           |
|   Driver for Logitech G915                                |
|                                                           |
|   Cheerpipe                                   20 Mar 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define LOGITECH_G915_COMMIT_BYTE       0x7F
#define LOGITECH_READ_TIMEOUT           300     //Timeout in ms

enum
{
    LOGITECH_G915_ZONE_MODE_KEYBOARD        = 0x01,
    LOGITECH_G915_ZONE_MODE_LOGO            = 0x00,
    LOGITECH_G915_ZONE_MODE_MULTIMEDIA      = 0x02,
    LOGITECH_G915_ZONE_MODE_GKEYS           = 0x03,
    LOGITECH_G915_ZONE_MODE_MODIFIERS       = 0x04
};

enum
{
    LOGITECH_G915_ZONE_FRAME_TYPE_LITTLE    = 0x1F,
    LOGITECH_G915_ZONE_FRAME_TYPE_BIG       = 0x6F,
    LOGITECH_G915_ZONE_FRAME_TYPE_MODE      = 0x1E
};

enum
{
    LOGITECH_G915_ZONE_DIRECT_KEYBOARD      = 0x01,
    LOGITECH_G915_ZONE_DIRECT_MEDIA         = 0x02,
    LOGITECH_G915_ZONE_DIRECT_LOGO          = 0x10,
    LOGITECH_G915_ZONE_DIRECT_INDICATORS    = 0x40,
};

enum
{
    LOGITECH_G915_MODE_OFF                  = 0x00,
    LOGITECH_G915_MODE_STATIC               = 0x01,
    LOGITECH_G915_MODE_BREATHING            = 0x02,
    LOGITECH_G915_MODE_CYCLE                = 0x03,
    LOGITECH_G915_MODE_WAVE                 = 0x04,
    LOGITECH_G915_MODE_RIPPLE               = 0x05,
    LOGITECH_G915_MODE_DIRECT               = 0xFF,
};

enum
{
    LOGITECH_G915_LOGO_MODE_OFF             = 0x00,
    LOGITECH_G915_LOGO_MODE_STATIC          = 0x01,
    LOGITECH_G915_LOGO_MODE_CYCLE           = 0x02,
    LOGITECH_G915_LOGO_MODE_BREATHING       = 0x03,
};

/*---------------------------------------------------------------------------------------------*\
| Speed is 1000 for fast and 20000 for slow.                                                    |
| Values are multiplied by 100 later to give lots of GUI steps.                                 |
\*---------------------------------------------------------------------------------------------*/
enum
{
    LOGITECH_G915_SPEED_SLOWEST             = 0xC8,     /* Slowest speed                       */
    LOGITECH_G915_SPEED_NORMAL              = 0x32,     /* Normal speed                        */
    LOGITECH_G915_SPEED_FASTEST             = 0x0A,     /* Fastest speed                       */
};

/* Ripple speeds are in ms directly. */
enum
{
    LOGITECH_G915_SPEED_RIPPLE_SLOW         = 200,
    LOGITECH_G915_SPEED_RIPPLE_NORMAL       = 20,
    LOGITECH_G915_SPEED_RIPPLE_FAST         = 2,
};

class LogitechG915Controller
{
public:
    LogitechG915Controller(hid_device* dev_handle, bool wired, std::string dev_name);
    ~LogitechG915Controller();

    std::string GetNameString();
    std::string GetSerialString();

    void        Commit();
    void        InitializeDirect();
    void        InitializeModeSet();
    void        BeginModeSet();
    void        SetDirect
                    (
                    unsigned char       frame_type,
                    unsigned char *     frame_data,
                    size_t              length
                    );
    void        SendSingleLed
                    (
                     unsigned char       keyCode,
                     unsigned char       r,
                     unsigned char       g,
                     unsigned char       b
                    );
    void        SetMode
                    (
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned short      brightness,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

private:
    hid_device*     dev_handle;
    unsigned char   feature_4522_idx;
    unsigned char   device_index;
    unsigned char   feature_8040_idx;
    unsigned char   feature_8071_idx;
    unsigned char   feature_8081_idx;
    std::string     name;

    void        SendDirectFrame
                    (
                    unsigned char       frame_type,
                    unsigned char *     frame_data,
                    size_t              length
                    );

    void        SendMode
                    (
                    unsigned char       zone,
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned short      brightness,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );
    void        SendCommit();
};

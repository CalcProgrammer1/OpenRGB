/*---------------------------------------------------------*\
| LogitechGLightsyncController.h                            |
|                                                           |
|   Driver for Logitech Lightsync                           |
|                                                           |
|   TheRogueZeta                                21 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <memory>
#include <string>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    LOGITECH_G_LIGHTSYNC_MODE_OFF               = 0x00,
    LOGITECH_G_LIGHTSYNC_MODE_STATIC            = 0x01,
    LOGITECH_G_LIGHTSYNC_MODE_CYCLE             = 0x02,
    LOGITECH_G_LIGHTSYNC_MODE_BREATHING         = 0x03,
};

/*---------------------------------------------------------------------------------------------*\
| Speed is 1000 for fast and 20000 for slow.                                                    |
| Values are multiplied by 100 later to give lots of GUI steps.                                 |
\*---------------------------------------------------------------------------------------------*/
enum
{
    LOGITECH_G_LIGHTSYNC_SPEED_SLOWEST          = 0xC8,  /* Slowest speed                   */
    LOGITECH_G_LIGHTSYNC_SPEED_NORMAL           = 0x32,  /* Normal speed                    */
    LOGITECH_G_LIGHTSYNC_SPEED_FASTEST          = 0x0A,  /* Fastest speed                   */
};

class LogitechGLightsyncController
{
public:
    LogitechGLightsyncController
        (
        hid_device*                 ev_cmd_handle,
        hid_device*                 ev_handle,
        const char*                 ath,
        unsigned char               id_dev_index,
        unsigned char               id_feature_index,
        unsigned char               id_fctn_ase_id,
        std::string                 ev_name
        );

    LogitechGLightsyncController
        (
        hid_device*                 dev_cmd_handle,
        hid_device*                 dev_handle,
        const char*                 path,
        unsigned char               hid_dev_index,
        unsigned char               hid_feature_index,
        unsigned char               hid_fctn_ase_id,
        std::shared_ptr<std::mutex> mutex_ptr,
        std::string                 dev_name
        );

    ~LogitechGLightsyncController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void        UpdateMouseLED
                   (
                    unsigned char       mode,
                    unsigned short      speed,
                    unsigned char       zone,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue,
                    unsigned char       brightness
                   );
    void        SetDirectMode(bool direct);

private:
    hid_device*                 dev;
    hid_device*                 cmd_dev;
    std::string                 location;
    std::string                 name;
    unsigned char               dev_index;
    unsigned char               feature_index;
    unsigned char               fctn_ase_id;
    std::shared_ptr<std::mutex> mutex;
};

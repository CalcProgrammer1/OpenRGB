/*---------------------------------------------------------*\
| RoccatSenseAimoController.h                               |
|                                                           |
|   Driver for Roccat Sense Aimo                            |
|                                                           |
|   Mola19                                      09 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include "RGBController.h"

enum
{
    ROCCAT_SENSE_AIMO_MODE_DIRECT     = 0x0B,
    ROCCAT_SENSE_AIMO_MODE_STATIC     = 0x01,
    ROCCAT_SENSE_AIMO_MODE_BREATHING  = 0x03,
    ROCCAT_SENSE_AIMO_MODE_HEARTBEAT  = 0x04,
    ROCCAT_SENSE_AIMO_MODE_DEFAULT    = 0x09,
    ROCCAT_SENSE_AIMO_MODE_WAVE       = 0x0A
};

enum
{
    ROCCAT_SENSE_AIMO_SPEED_MIN             = 0xFF,
    ROCCAT_SENSE_AIMO_SPEED_MAX             = 0x00,
    ROCCAT_SENSE_AIMO_SPEED_DEFAULT         = 0x07,
    ROCCAT_SENSE_AIMO_BRIGHTNESS_MIN        = 0x00,
    ROCCAT_SENSE_AIMO_BRIGHTNESS_MAX        = 0xFF,
    ROCCAT_SENSE_AIMO_BRIGHTNESS_DEFAULT    = 0xFF
};

struct mode_struct
{
    uint8_t     profile;
    uint8_t     mode;
    uint8_t     speed;
    uint8_t     brightness;
    RGBColor    left;
    RGBColor    right;
};

class RoccatSenseAimoController
{
public:
    RoccatSenseAimoController(hid_device* dev_handle, char *path, std::string dev_name);
    ~RoccatSenseAimoController();

    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerial();
    std::string     GetVersion();

    mode_struct     GetMode();

    void            SendDirect(std::vector<RGBColor> colors);
    void            SetMode(uint8_t profile, uint8_t mode, uint8_t speed, uint8_t brightness, std::vector<RGBColor> colors);

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
};

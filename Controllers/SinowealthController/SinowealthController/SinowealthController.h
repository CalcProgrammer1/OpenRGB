/*---------------------------------------------------------*\
| SinowealthController.h                                    |
|                                                           |
|   Driver for Sinowealth mice, including Glorious          |
|                                                           |
|   Niels Westphal (crashniels)                 20 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include <hidapi.h>
#include "RGBController.h"

#define SINOWEALTH_CONFIG_SIZE         167
#define SINOWEALTH_CONFIG_SIZE_MIN     131
#define SINOWEALTH_CONFIG_REPORT_SIZE  520
#define SINOWEALTH_COMMAND_REPORT_SIZE   6

enum
{
    GLORIOUS_MODE_OFF               = 0x00, //does nothing
    GLORIOUS_MODE_RAINBOW           = 0x01,
    GLORIOUS_MODE_STATIC            = 0x02,
    GLORIOUS_MODE_SPECTRUM_BREATING = 0x03,
    GLORIOUS_MODE_TAIL              = 0x04,
    GLORIOUS_MODE_SPECTRUM_CYCLE    = 0x05,
    GLORIOUS_MODE_RAVE              = 0x07,
    GLORIOUS_MODE_EPILEPSY          = 0x08, //not in the official software
    GLORIOUS_MODE_WAVE              = 0x09,
    GLORIOUS_MODE_BREATHING         = 0x0a,
};

enum
{
    GLORIOUS_SPEED_SLOW             = 0x41,
    GLORIOUS_SPEED_NORMAL           = 0x42,
    GLORIOUS_SPEED_FAST             = 0x43,
};

enum
{
    GLORIOUS_DIRECTION_UP           = 0x00,
    GLORIOUS_DIRECTION_DOWN         = 0x01,
};

enum
{
    GLORIOUS_MODE_BREATING_SLOW     = 0x01,
    GLORIOUS_MODE_BREATING_NORMAL   = 0x02,
    GLORIOUS_MODE_BREATING_FAST     = 0x03,
};

class SinowealthController
{
public:
    SinowealthController(hid_device* dev_data_handle, hid_device* dev_cmd_handle, char *_path, std::string dev_name); //RGB, Command, path
    ~SinowealthController();

    unsigned int    GetLEDCount();
    std::string     GetLocation();
    std::string     GetName();
    std::string     GetSerialString();
    std::string     GetFirmwareVersion();

    void            SetLEDColor(RGBColor* color_buf);
    void            SetMode(unsigned char mode, unsigned char speed, unsigned char direction, RGBColor* color_buf);
    int             GetProfile();
private:
    hid_device*     dev_cmd;
    hid_device*     dev_data;
    unsigned int    led_count;
    unsigned char   current_mode;
    unsigned char   current_speed;
    unsigned char   current_direction;
    unsigned char   device_configuration[SINOWEALTH_CONFIG_REPORT_SIZE];
    std::string     location;
    std::string     name;
};

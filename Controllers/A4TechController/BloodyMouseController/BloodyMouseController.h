/*---------------------------------------------------------*\
| BloodyMouseController.h                                   |
|                                                           |
|   Driver for A4Tech Bloody Mouse                          |
|                                                           |
|   Chris M (Dr_No)                             30 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
\*-----------------------------------------------------*/
#define BLOODY_W60_PRO_PID                          0x37EA
#define BLOODY_W70_MAX_PID                          0x79EF
#define BLOODY_W90_MAX_PID                          0x3666
#define BLOODY_W90_PRO_PID                          0x39B6

/*-----------------------------------------------------*\
| Mousemat product IDs                                  |
\*-----------------------------------------------------*/
#define BLOODY_MP_50RS_PID                          0xFA60

#define HID_MAX_STR                     255
#define BLOODYMOUSE_WRITE_PACKET_SIZE   64

#define BLOODYMOUSE_BRIGHTNESS_MIN      0
#define BLOODYMOUSE_BRIGHTNESS_MAX      255

enum
{
    BLOODYMOUSE_MODE_DIRECT             = 0x01, //Direct Led Control - Independently set LEDs in zone
};

enum
{
    BLOODYMOUSE_REPORT_BYTE             = 1,
    BLOODYMOUSE_COMMAND_BYTE            = 2,
    BLOODYMOUSE_MODE_BYTE               = 3,
    BLOODYMOUSE_DATA_BYTE               = 8,
};

class BloodyMouseController
{
public:
    BloodyMouseController(hid_device* dev_handle, const char* path, uint16_t product_id, std::string dev_name);
    ~BloodyMouseController();

    uint16_t        GetPid();
    std::string     GetSerial();
    std::string     GetLocation();
    std::string     GetName();

    void            SetLedsDirect(std::vector<RGBColor> colors);

private:
    uint16_t        pid;
    std::string     location;
    std::string     name;
    hid_device*     dev;

    void            InitDevice();
};

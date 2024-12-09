/*---------------------------------------------------------*\
| CMMM712Controller.h                                       |
|                                                           |
|   Driver for Cooler Master MM712 mouse                    |
|   Derived from CMMM711Controller.h                        |
|                                                           |
|   Chris M (Dr_No)                             14 Feb 2021 |
|   Frans Meulenbroeks                          08 Dec 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    CM_MM712_MODE_STATIC            = 0,  //Static Mode
    CM_MM712_MODE_BREATHING         = 1,  //Breathing Mode
    CM_MM712_MODE_SPECTRUM_CYCLE    = 2,  //Spectrum Cycle Mode
    CM_MM712_MODE_OFF               = 3,  //Turn Off
    CM_MM712_MODE_DIRECT            = 4,  //Direct LED Control
};

enum
{
    CM_MM712_SPEED_SLOWEST          = 0x0, //Slowest speed
    CM_MM712_SPEED_NORMAL           = 0x2, //Normal speed
    CM_MM712_SPEED_FASTEST          = 0x4, //Fastest speed
};

class CMMM712Controller
{
public:
    CMMM712Controller(hid_device* dev_handle, char *_path);
    ~CMMM712Controller();

    std::string GetDeviceName();
    std::string GetSerial();
    std::string GetLocation();

    uint8_t     GetMode();
    RGBColor    GetInitialLedColor();

    void        SendUpdate(uint8_t mode, uint8_t speed, RGBColor color, uint8_t brightness);
    void        SetMode(uint8_t mode);
    void        SetDirectMode(bool onoff);
    void        SetLedsDirect(RGBColor color);
    void        SaveStatus();
private:
    std::string device_name;
    std::string serial;
    std::string location;
    hid_device* dev;

    uint8_t     current_mode;
    RGBColor    initial_color;

    void        GetColorStatus(uint8_t mode);
    void        GetModeStatus();
    void        SendInitPacket();
    void        SetProfile(uint8_t profile);
    void        SendBuffer(uint8_t *buffer, uint8_t buffer_size);

};

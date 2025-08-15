/*---------------------------------------------------------*\
| VS_XG270QG_Controller.h                                   |
|                                                           |
|   Driver for ViewSonic XG270QG                            |
|                                                           |
|   Lanzaa                                      23 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <cstdint>
#include <mutex>
#include <string>
#include <hidapi.h>
#include "RGBController.h"

enum
{
    VS_MODE_OFF         = 0x00,
    VS_MODE_STATIC      = 0x01,
    VS_MODE_BREATHING   = 0x02,
    VS_MODE_RAINBOW     = 0x07,
    VS_MODE_ELITE       = 0x0A,
    VS_MODE_JAZZ        = 0x0C,
    VS_MODE_WATERFALL   = 0x12,
    VS_MODE_ELITEGLOBAL = 0x13,
};

class VS_XG270QG_Controller
{
public:
    VS_XG270QG_Controller(hid_device* device, const char* path, std::string dev_name);
    ~VS_XG270QG_Controller();

    std::string GetLocation();
    std::string GetName();
    std::string GetSerial();

    void        SetMode(uint8_t mode1, uint8_t r1, uint8_t g1, uint8_t b1,
                        uint8_t mode2, uint8_t r2, uint8_t g2, uint8_t b2);

private:
    hid_device* dev;
    std::string location;
    std::string name;
    std::string serial;

    std::string ReadVersion();
    void        SendModeComplete
                    (
                    uint8_t mode1, uint8_t r1, uint8_t g1, uint8_t b1,
                    uint8_t mode2, uint8_t r2, uint8_t g2, uint8_t b2
                    );
    void        SendCommand(uint8_t  *config, size_t length);
};

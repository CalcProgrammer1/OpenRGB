/*---------------------------------------------------------*\
| VS_XG270QC_Controller.h                                   |
|                                                           |
|   Driver for ViewSonic XG270QC                            |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <cstdint>
#include <mutex>
#include <string>
#include <hidapi.h>
#include "RGBController.h"

class VS_XG270QC_Controller
{
public:
    enum ModeValues
    {
        VS_MODE_OFF          = 0x00,
        VS_MODE_STATIC       = 0x01,
        VS_MODE_BREATHING    = 0x02,
        VS_MODE_WARP_SPEED   = 0x06,
        VS_MODE_RAINBOW      = 0x07,
        VS_MODE_STACK        = 0x09,
        VS_MODE_MUSIC        = 0x12,
        VS_MODE_MUSIC_Z2     = 0x13,  // Zone 2 value for Music mode
        VS_MODE_MUSIC_PULSE  = 0x12,  // Same as Music for zone 1
        VS_MODE_MUSIC_PULSE_Z2 = 0x14, // Zone 2 value for Music Pulse mode
    };

    VS_XG270QC_Controller(hid_device* device, const char* path, std::string dev_name);
    ~VS_XG270QC_Controller();

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

    void        SendModeComplete
                    (
                    uint8_t mode1, uint8_t r1, uint8_t g1, uint8_t b1,
                    uint8_t mode2, uint8_t r2, uint8_t g2, uint8_t b2
                    );
    void        SendCommand(uint8_t  *config, size_t length);
};

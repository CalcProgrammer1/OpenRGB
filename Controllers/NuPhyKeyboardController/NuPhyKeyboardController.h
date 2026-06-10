/*---------------------------------------------------------*\
| NuPhyKeyboardController.h                                 |
|                                                           |
|   Driver for NuPhy QMK/VIA RGB Matrix keyboards           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include <string>

class NuPhyKeyboardController
{
public:
    NuPhyKeyboardController(hid_device* dev_handle, const char* path, const std::string& device_name);
    ~NuPhyKeyboardController();

    std::string GetLocation();
    std::string GetName();
    std::string GetSerial();
    std::string GetVersion();
    bool        GetSupported();

    unsigned char GetBrightness();
    unsigned char GetEffect();
    unsigned char GetSpeed();
    void          GetColor(unsigned char* hue, unsigned char* saturation);

    void SetBrightness(unsigned char brightness);
    void SetEffect(unsigned char effect);
    void SetSpeed(unsigned char speed);
    void SetColor(unsigned char hue, unsigned char saturation);
    void Save();

private:
    hid_device*     dev;
    std::string     location;
    std::string     name;
    std::string     serial;
    bool            supported;
    unsigned short  via_protocol_version;

    bool SendCommand(unsigned char command, unsigned char channel, unsigned char value_id, unsigned char* value, unsigned char value_size);
};

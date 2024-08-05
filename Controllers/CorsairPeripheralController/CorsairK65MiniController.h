/*---------------------------------------------------------*\
| CorsairK65MiniController.cpp                              |
|                                                           |
|   Driver for Corsair K65 Mini keyboard                    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define PACKET_LENGTH 1025
#define K65_WRITE_COMMAND 0x08

class CorsairK65MiniController
{
public:
    CorsairK65MiniController(hid_device* dev_handle, const char* path);
    ~CorsairK65MiniController();

    std::string             GetDeviceLocation();
    std::string             GetFirmwareString();
    std::string             GetName();
    std::string             GetSerialString();
    void                    SetLEDs(std::vector<RGBColor> colors, std::vector<unsigned int> positions);

private:
    hid_device*             dev;

    std::string             firmware_version;
    std::string             location;

    void                    LightingControl();
};

/*---------------------------------------------------------*\
| HyperXMicrophoneV2Controller.cpp                          |
|                                                           |
|   Driver for HyperX QuadCast 2S microphone                |
|                                                           |
|   Morgan Guimard (morg)                                   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <hidapi.h>
#include "RGBController.h"

#define HYPERX_QUADCAST_2S_PACKET_SIZE      64
#define HYPERX_QUADCAST_2S_REPORT_ID        0x44
#define HYPERX_QUADCAST_2S_LEDS_PER_PACKET  20
#define HYPERX_QUADCAST_2S_MATRIX_WIDTH     12
#define HYPERX_QUADCAST_2S_MATRIX_HEIGHT    9
#define HYPERX_QUADCAST_2S_TOTAL_LEDS       108

class HyperXMicrophoneV2Controller
{
public:
    HyperXMicrophoneV2Controller(hid_device* dev, std::string path, std::string dev_name);
    ~HyperXMicrophoneV2Controller();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void SendDirect(std::vector<RGBColor> color_data);

private:
    hid_device*         dev;
    std::string         location;
    std::string         name;
    std::mutex          lock;
};

/*---------------------------------------------------------*\
| HyperXMicrophoneController.cpp                            |
|                                                           |
|   Driver for HyperX microphone                            |
|                                                           |
|   Matt Silva (thesilvanator)                              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include "hidapi_wrapper.h"
#include "RGBController.h"

#define HYPERX_QUADCAST_S_PACKET_SIZE 64 + 1
#define HYPERX_QUADCAST_S_FRAME_SIZE  8

class HyperXMicrophoneController
{
public:
    HyperXMicrophoneController(hidapi_wrapper hid_wrapper, hid_device* dev, std::string path, std::string dev_name);
    ~HyperXMicrophoneController();

    std::string GetDeviceLocation();
    std::string GetNameString();
    std::string GetSerialString();

    void SendDirect(std::vector<RGBColor> color_data);
    void SaveColors(std::vector<RGBColor> colors, unsigned int num_frames);

private:
    hidapi_wrapper      wrapper;
    hid_device*         dev;
    std::string         location;
    std::mutex          lock;
    std::string         name;

    void                SendEOT(uint8_t frame_count);
    void                SendToRegister(uint8_t reg, uint8_t param1, uint8_t param2);
};

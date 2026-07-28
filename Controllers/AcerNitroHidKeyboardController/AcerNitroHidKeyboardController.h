/*---------------------------------------------------------*\
| RGBController_AcerNitroHidKeyboard.cpp                    |
|                                                           |
|   Driver for Acer Nitro Keyboard                          |
|                                                           |
|   Marek Pokropiński                           Jul 27 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

enum
{
    ACER_HID_MODE_DIRECT    = 1,
    ACER_HID_MODE_STATIC    = 2,
    ACER_HID_MODE_BREATHING = 4,
    ACER_HID_MODE_NEON      = 5,
    ACER_HID_MODE_OPMODE    = 6,
    ACER_HID_MODE_WAVE      = 7,
    ACER_HID_MODE_SHIFTING  = 8,
    ACER_HID_MODE_ZOOM      = 9,
    ACER_HID_MODE_METEOR    = 10,
    ACER_HID_MODE_TWINKLING = 11,
};

#define ACER_HID_NAME_BUFFER_SIZE 255
#define ACER_HID_NUM_ZONES 4
#define ACER_HID_PACKET_SIZE 11

#include <mutex>
#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBControllerInterface.h"

class AcerNitroHidKeyboardController
{
public:
    AcerNitroHidKeyboardController(hid_device *dev_handle, const char *path);
    ~AcerNitroHidKeyboardController();

    std::string GetDeviceName() const;
    std::string GetSerial() const;
    std::string GetLocation() const;

    void SetLEDs(const std::vector<RGBColor> &colors, unsigned char brightness, unsigned char device_id);
    void SetAllLEDs(const RGBColor &color, unsigned char brightness, unsigned char speed, unsigned char direction, unsigned char mode, unsigned char device_id);

private:
    hid_device *dev;
    std::string device_name;
    std::string location;
    std::mutex device_mutex;
};

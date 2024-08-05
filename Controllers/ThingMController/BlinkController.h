/*---------------------------------------------------------*\
| BlinkController.h                                         |
|                                                           |
|   Driver for ThingM Blink                                 |
|                                                           |
|   Eric S (edbgon)                             01 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <array>
#include <string>
#include <hidapi.h>

#define BLINK_PACKET_SIZE       9  //Includes extra first byte for non HID Report packets

#define BLINK_MODE_OFF          0
#define BLINK_MODE_DIRECT       1
#define BLINK_MODE_FADE         2

#define HID_MAX_STR             255

class BlinkController
{
public:
    BlinkController(hid_device* dev_handle, char *_path);
    ~BlinkController();

    std::string   GetDeviceName();
    std::string   GetSerial();
    std::string   GetLocation();

    unsigned char GetLedRed();
    unsigned char GetLedGreen();
    unsigned char GetLedBlue();
    unsigned char GetLedSpeed();
    unsigned char GetBrightness();
    void          SendUpdate(unsigned char led, unsigned char red, unsigned char green, unsigned char blue, unsigned int speed);

private:
    std::string             device_name;
    std::string             serial;
    std::string             location;
    hid_device*             dev;

    unsigned char           current_red;
    unsigned char           current_green;
    unsigned char           current_blue;

    void SendUpdate();
};

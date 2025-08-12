/*---------------------------------------------------------*\
| Lenovo4ZoneUSBController.h                                |
|                                                           |
|   Driver for Lenovo 4-Zone devices                        |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <array>
#include <string>
#include <utility>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"
#include "LogManager.h"
#include "LenovoDevices4Zone.h"

#ifndef HID_MAX_STR
#define HID_MAX_STR                255
#endif

#define LENOVO_4_ZONE_HID_PACKET_SIZE     33

class Lenovo4ZoneUSBController
{
    public:
    /*--------------*\
    |ctor(s) and dtor|
    \*--------------*/
    Lenovo4ZoneUSBController(hid_device* dev_handle, const char* path, uint16_t in_pid, std::string dev_name);
    ~Lenovo4ZoneUSBController();

    void setMode(const KeyboardState &in_mode);

    /*--------------*\
    |device functions|
    \*--------------*/
    uint16_t getPid();
    std::string getName();
    std::string getLocation();

    private:
    /*--------------*\
    |data members    |
    \*--------------*/
    std::string name;
    hid_device *dev;
    std::string location;
    uint16_t pid;
    KeyboardState mode;

    /*--------------*\
    |device functions|
    \*--------------*/
    void sendBasicInstruction(uint8_t instruction);
};

/*---------------------------------------------------------*\
| RGBController_Lenovo_USB.h                                |
|                                                           |
|   RGBController for Lenovo USB devices                    |
|                                                           |
|   Cooper Hall (geobot19)                      17 Apr 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "LenovoDevices.h"
#include "LenovoUSBController.h"
#include "RGBController.h"

#define NA 0xFFFFFFFF

class RGBController_LenovoUSB : public RGBController
{
public:
    RGBController_LenovoUSB(LenovoUSBController* controller_ptr);
    ~RGBController_LenovoUSB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    std::string ConvertBytesToHex(const std::vector<uint8_t> &input);
    std::string keyboardToString(LENOVO_KEYBOARD kb);
    std::string sizeToString(LENOVO_SIZE size);
    void        DumpControllerInformation();

    LENOVO_KEYBOARD keyboard_type;
    LENOVO_SIZE chasis_size;

    LenovoUSBController *controller;
};

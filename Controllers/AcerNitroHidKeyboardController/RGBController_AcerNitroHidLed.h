/*---------------------------------------------------------*\
| RGBController_AcerNitroHidLed.cpp                         |
|                                                           |
|   RGBController for Acer Nitro HID LED                    |
|                                                           |
|   Marek Pokropiński                           Jul 27 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AcerNitroHidKeyboardController.h"
#include "RGBController_AcerNitroHidKeyboard.h"

class RGBController_AcerNitroHidLed : public RGBController
{
public:
    RGBController_AcerNitroHidLed(std::shared_ptr<AcerNitroHidKeyboardController> controller);
    ~RGBController_AcerNitroHidLed();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    std::shared_ptr<AcerNitroHidKeyboardController> controller;
    unsigned int device_id;
};

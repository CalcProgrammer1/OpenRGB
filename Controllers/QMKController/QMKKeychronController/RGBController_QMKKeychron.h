/*---------------------------------------------------------*\
| RGBController_QMKKeychron.h                               |
|                                                           |
|   RGBController for Keychron QMK-based keyboards          |
|   (Q1 HE and other KEYCHRON_RGB-enabled models)           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "QMKKeychronController.h"

class RGBController_QMKKeychron : public RGBController
{
public:
    RGBController_QMKKeychron(QMKKeychronController* controller_ptr);
    ~RGBController_QMKKeychron();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    QMKKeychronController* controller;

    static void RGBToHSV(unsigned char r, unsigned char g, unsigned char b, unsigned char& h, unsigned char& s, unsigned char& v);
};

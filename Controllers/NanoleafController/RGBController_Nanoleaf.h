/*---------------------------------------------------------*\
| RGBController_Nanoleaf.h                                  |
|                                                           |
|   RGBController for Nanoleaf                              |
|                                                           |
|   Nikita Rushmanov                            13 Jan 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NanoleafController.h"

class RGBController_Nanoleaf : public RGBController
{

public:
    RGBController_Nanoleaf(std::string a_address, int a_port, std::string a_auth_token);
    ~RGBController_Nanoleaf();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    NanoleafController controller;
};

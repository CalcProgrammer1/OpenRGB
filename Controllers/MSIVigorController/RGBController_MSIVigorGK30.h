/*---------------------------------------------------------*\
| RGBController_MSIVigorGK30.h                              |
|                                                           |
|   RGBController for MSI Vigor GK30                        |
|                                                           |
|   Morgan Guimard (morg)                       01 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIVigorGK30Controller.h"

class RGBController_MSIVigorGK30 : public RGBController
{
public:
    RGBController_MSIVigorGK30(MSIVigorGK30Controller* controller_ptr);
    ~RGBController_MSIVigorGK30();

    void SetupZones();

    void DeviceUpdateLEDs();
    void DeviceUpdateZoneLEDs(int zone);
    void DeviceUpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    MSIVigorGK30Controller* controller;
};

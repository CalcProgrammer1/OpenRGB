/*---------------------------------------------------------*\
| RGBController_MSIVigorGK30.h                              |
|                                                           |
|   RGBController for MSI Vigor GK30                        |
|                                                           |
|   Morgan Guimard (morg)                       01 Jun 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    MSIVigorGK30Controller* controller;
};

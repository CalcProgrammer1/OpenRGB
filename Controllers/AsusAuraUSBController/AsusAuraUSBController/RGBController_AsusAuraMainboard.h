/*---------------------------------------------------------*\
| RGBController_AsusAuraMainboard.h                         |
|                                                           |
|   RGBController for ASUS Aura mainboard                   |
|                                                           |
|   rytypete                                    30 Aug 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController_AsusAuraUSB.h"
#include "AsusAuraMainboardController.h"

class RGBController_AuraMainboard : public RGBController_AuraUSB
{
public:
    RGBController_AuraMainboard(AuraMainboardController* controller_ptr);

    void        DeviceSaveMode();

private:
    void        DeviceUpdateShutdownEffect();
};

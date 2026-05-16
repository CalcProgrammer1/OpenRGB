/*---------------------------------------------------------*\
| RGBController_Virtual.h                                   |
|                                                           |
|   Virtual RGBController that can be filled out by a       |
|   plugin, including function pointers for implementing    |
|   functionality plugin-side.                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 May 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"

class RGBController_Virtual : public RGBController
{
public:
    RGBController_Virtual(RGBController_Setup* setup);
    ~RGBController_Virtual();

    void                UpdateVirtualController(RGBController_Setup* setup);

    void                DeviceConfigureZone(int zone_idx);

    void                DeviceUpdateLEDs();
    void                DeviceUpdateZoneLEDs(int zone);
    void                DeviceUpdateSingleLED(int led);

    void                DeviceUpdateMode();
    void                DeviceUpdateZoneMode(int zone);
    void                DeviceSaveMode();

    void                DeviceUpdateDeviceSpecificConfiguration();
    void                DeviceUpdateDeviceSpecificZoneConfiguration(int zone);

private:
    /*-----------------------------------------------------*\
    | Pointer to user object                                |
    \*-----------------------------------------------------*/
    void*               object_ptr;

    /*-----------------------------------------------------*\
    | Function pointers                                     |
    \*-----------------------------------------------------*/
    void                (*DeviceConfigureZone_ptr)(void*, int);

    void                (*DeviceUpdateLEDs_ptr)(void*);
    void                (*DeviceUpdateZoneLEDs_ptr)(void*, int);
    void                (*DeviceUpdateSingleLED_ptr)(void*, int);

    void                (*DeviceUpdateMode_ptr)(void*);
    void                (*DeviceSaveMode_ptr)(void*);
    void                (*DeviceUpdateZoneMode_ptr)(void*, int);

    void                (*DeviceUpdateDeviceSpecificConfiguration_ptr)(void*);
    void                (*DeviceUpdateDeviceSpecificZoneConfiguration_ptr)(void*, int);

    void                SetupLEDs(RGBController_Setup* setup);
};

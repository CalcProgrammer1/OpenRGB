/*-----------------------------------------*\
|  RGBController_ENESMBus.h                 |
|                                           |
|  Generic RGB Interface for ENE SMBus      |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/13/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ENESMBusController.h"

class RGBController_ENESMBus : public RGBController
{
public:
    RGBController_ENESMBus(ENESMBusController* controller_ptr);
    ~RGBController_ENESMBus();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    ENESMBusController* controller;

    int         GetDeviceMode();
};

/*-------------------------------------------------------------------*\
|  RGBController_Lenovo4ZoneUSB.h                                     |
|                                                                     |
|  interface for Lenovo 4-Zones Devices                               |
\*-------------------------------------------------------------------*/

#pragma once

#include "LenovoDevices.h"
#include "Lenovo4ZoneUSBController.h"
#include "RGBController.h"

#include <vector>

#define NA 0xFFFFFFFF

class RGBController_Lenovo4ZoneUSB : public RGBController
{
public:
    RGBController_Lenovo4ZoneUSB(Lenovo4ZoneUSBController* controller_ptr);
    ~RGBController_Lenovo4ZoneUSB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    KeyboardState state;

    Lenovo4ZoneUSBController *controller;
};

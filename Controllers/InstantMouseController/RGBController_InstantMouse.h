/*-----------------------------------------*\
|  RGBController_InstantMouse.h             |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Instant mouse RGB USB Driver             |
|                                           |
|  Guimard Morgan (morg) 1/19/2024          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "InstantMouseController.h"

class RGBController_InstantMouse : public RGBController
{
public:
    RGBController_InstantMouse(InstantMouseController* controller_ptr);
    ~RGBController_InstantMouse();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    InstantMouseController* controller;
};

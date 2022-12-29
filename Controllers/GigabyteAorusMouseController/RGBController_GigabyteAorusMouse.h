/*-----------------------------------------*\
|  RGBController_GigabyteAorusMouse.h       |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte Aorus Mouse lighting            |
|  USB Driver                               |
|                                           |
|  Guimard Morgan (morg) 12/29/2022         |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteAorusMouseController.h"

class RGBController_GigabyteAorusMouse : public RGBController
{
public:
    RGBController_GigabyteAorusMouse(GigabyteAorusMouseController* controller_ptr);
    ~RGBController_GigabyteAorusMouse();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    GigabyteAorusMouseController* controller;
};

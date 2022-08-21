/*-----------------------------------------*\
|  RGBController_NvidiaESA.h                |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Nvidia ESA RGB USB Driver                |
|                                           |
|  Guimard Morgan (morg) 2/18/2022          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "NvidiaESAController.h"

class RGBController_NvidiaESA : public RGBController
{
public:
    RGBController_NvidiaESA(NvidiaESAController* controller_ptr);
    ~RGBController_NvidiaESA();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    NvidiaESAController* controller;
};

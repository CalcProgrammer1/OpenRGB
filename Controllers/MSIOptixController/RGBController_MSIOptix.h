/*-----------------------------------------*\
|  RGBController_MSIOptix.h                 |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  MISOptix RGB USB Driver                  |
|                                           |
|  Guimard Morgan (morg) 1/10/2022          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "MSIOptixController.h"

class RGBController_MSIOptix : public RGBController
{
public:
    RGBController_MSIOptix(MSIOptixController* controller_ptr);
    ~RGBController_MSIOptix();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    MSIOptixController* controller;
};

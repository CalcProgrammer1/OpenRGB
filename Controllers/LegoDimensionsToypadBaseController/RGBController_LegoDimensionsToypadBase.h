/*-----------------------------------------*\
|  RGBController_LegoDimensionsToypadBase.h |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Lego Dimensions Toypad Base USB Driver   |
|                                           |
|  Guimard Morgan (morg) 06/02/2023         |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LegoDimensionsToypadBaseController.h"

class RGBController_LegoDimensionsToypadBase : public RGBController
{
public:
    RGBController_LegoDimensionsToypadBase(LegoDimensionsToypadBaseController* controller_ptr);
    ~RGBController_LegoDimensionsToypadBase();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LegoDimensionsToypadBaseController* controller;
};

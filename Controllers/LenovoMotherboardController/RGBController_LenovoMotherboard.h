/*-----------------------------------------*\
|  RGBController_LenovoMotherboard.h        |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Lenovo motherboards RGB USB Driver       |
|                                           |
|  Guimard Morgan (morg) 12/26/2022         |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LenovoMotherboardController.h"

class RGBController_LenovoMotherboard : public RGBController
{
public:
    RGBController_LenovoMotherboard(LenovoMotherboardController* controller_ptr);
    ~RGBController_LenovoMotherboard();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    LenovoMotherboardController* controller;
};

/*---------------------------------------------*\
|  RGBController_PatriotViperSteel.h            |
|                                               |
|  Generic RGB Interface for OpenRGB            |
|  Patriot Viper Steel RGB interface            |
\*---------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "PatriotViperSteelController.h"

class RGBController_PatriotViperSteel : public RGBController
{
public:
    RGBController_PatriotViperSteel(PatriotViperSteelController *controller_ptr);
    ~RGBController_PatriotViperSteel();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    PatriotViperSteelController *controller;
};

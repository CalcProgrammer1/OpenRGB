/*-----------------------------------------*\
|  RGBController_RoccatKova.h               |
|                                           |
|  RGB Controller for Roccat Kova           |
|                                           |
|  Gustash 01/12/2022                       |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "RoccatKovaController.h"

class RGBController_RoccatKova : public RGBController
{
public:
    RGBController_RoccatKova(RoccatKovaController *controller_ptr);
    ~RGBController_RoccatKova();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    RoccatKovaController *controller;
};

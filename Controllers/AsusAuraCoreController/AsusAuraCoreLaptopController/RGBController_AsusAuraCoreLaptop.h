/*---------------------------------------------------------------------*\
|  RGBController_AsusAuraCoreLaptop.h                                   |
|                                                                       |
|  Driver for AsusAuraCoreLaptop USB Controller                         |
|                                                                       |
|  Chris M (Dr_No)          28 Jul 2022                                 |
|                                                                       |
\*---------------------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "AsusAuraCoreLaptopController.h"

class RGBController_AsusAuraCoreLaptop : public RGBController
{
public:
    RGBController_AsusAuraCoreLaptop(AsusAuraCoreLaptopController* controller_ptr);
    ~RGBController_AsusAuraCoreLaptop();

    void                SetupZones();
    void                ResizeZone(int zone, int new_size);

    void                DeviceUpdateLEDs();
    void                UpdateZoneLEDs(int zone);
    void                UpdateSingleLED(int led);

    void                DeviceUpdateMode();

private:
    void                Init_Controller();

    AsusAuraCoreLaptopController*      controller;
};

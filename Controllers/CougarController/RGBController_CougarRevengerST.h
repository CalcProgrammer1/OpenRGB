/*-----------------------------------------*\
|  RGBController_CougarRevengerST.h         |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Cougar Revenger ST USB Driver            |
|                                           |
|  Guimard Morgan (morg) 3/17/2022          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CougarRevengerSTController.h"

class RGBController_CougarRevengerST : public RGBController
{
public:
    RGBController_CougarRevengerST(CougarRevengerSTController* controller_ptr);
    ~RGBController_CougarRevengerST();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CougarRevengerSTController* controller;
};

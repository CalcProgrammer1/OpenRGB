/*-----------------------------------------*\
|  RGBController_CorsairVengeance.h         |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Corsair Vengeance RGB driver             |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/13/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairVengeanceController.h"

class RGBController_CorsairVengeance : public RGBController
{
public:
    RGBController_CorsairVengeance(CorsairVengeanceController* corsair_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        UpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        UpdateMode();

private:
    CorsairVengeanceController* corsair;
};

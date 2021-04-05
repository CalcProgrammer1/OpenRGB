/*-------------------------------------------------------------------*\
|  RGBController_CorsairHydroPlatinum.h                               |
|                                                                     |
|  Driver for Corsair Hydro Platinum AIO Coolers                      |
|                                                                     |
|  Kasper         28th March 2021                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CorsairHydroPlatinumController.h"

class RGBController_CorsairHydroPlatinum : public RGBController
{
public:
    RGBController_CorsairHydroPlatinum(CorsairHydroPlatinumController* corsair_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    CorsairHydroPlatinumController* corsair;

    void        Init_Controller();
};

/*-----------------------------------------*\
|  RGBController_CorsairHydro.h             |
|                                           |
|  Generic RGB Interface for Corsair        |
|  Hydro Series                             |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/17/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CorsairHydroController.h"

class RGBController_CorsairHydro : public RGBController
{
public:
    RGBController_CorsairHydro(CorsairHydroController* corsair_ptr);
    ~RGBController_CorsairHydro();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    CorsairHydroController*     corsair;
};

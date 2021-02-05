/*-----------------------------------------*\
|  RGBController_CorsairVengeancePro.h      |
|                                           |
|  Generic RGB Interface for OpenAuraSDK    |
|  Corsair Vengeance Pro RGB driver         |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/30/2019   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairVengeanceProController.h"

class RGBController_CorsairVengeancePro : public RGBController
{
public:
    RGBController_CorsairVengeancePro(CorsairVengeanceProController* corsair_ptr);
    ~RGBController_CorsairVengeancePro();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    CorsairVengeanceProController* corsair;
};

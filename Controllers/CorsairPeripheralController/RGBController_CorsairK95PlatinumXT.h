/*-----------------------------------------*\
|  RGBController_CorsairK95PlatinumXT.h     |
|                                           |
|                                           |
|  Guimard Morgan (morg) 6/07/2022          |
\*-----------------------------------------*/
#pragma once

#include "RGBController.h"
#include "CorsairK95PlatinumXTController.h"

class RGBController_CorsairK95PlatinumXT : public RGBController
{
public:
    RGBController_CorsairK95PlatinumXT(CorsairK95PlatinumXTController* controller_ptr);
    ~RGBController_CorsairK95PlatinumXT();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    CorsairK95PlatinumXTController*                              controller;    
};

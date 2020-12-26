/*-------------------------------------------*\
|  RGBController_HyperXPulsefireDart.h        |
|                                             |
|  Generic RGB Interface for HyperX           |
|  Pulsefire Dart                             |
|                                             |
|  Santeri Pikarinen (santeri3700) 12/26/2020 |
\*-------------------------------------------*/

#pragma once
#include <chrono>

#include "RGBController.h"
#include "HyperXPulsefireDartController.h"

class RGBController_HyperXPulsefireDart : public RGBController
{
public:
    RGBController_HyperXPulsefireDart(HyperXPulsefireDartController* hyperx_ptr);
    ~RGBController_HyperXPulsefireDart();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
    
private:
    HyperXPulsefireDartController*    hyperx;
};

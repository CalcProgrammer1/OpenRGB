/*--------------------------------------------*\
|  RGBController_CorsairDominatorPlatinum.h    |
|                                              |
|  Corsair Dominator Platinum RGB driver       |
|                                              |
|  Erik Gilling (konkers)       9/25/2020      |
\*--------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "CorsairDominatorPlatinumController.h"

class RGBController_CorsairDominatorPlatinum : public RGBController
{
public:
    RGBController_CorsairDominatorPlatinum(CorsairDominatorPlatinumController* corsair_ptr);
    ~RGBController_CorsairDominatorPlatinum();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void SetCustomMode();
    void DeviceUpdateMode();

private:
    CorsairDominatorPlatinumController* corsair;
};

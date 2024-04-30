/*-----------------------------------------*\
|  RGBController_LianLiUniHubAL.h           |
|                                           |
|  Generic RGB Interface for Lian Li Uni    |
|  Hub AL USB controller driver             |
|                                           |
|  Oliver P 04/26/2022                      |
|  Credit to Luca Lovisa for original work. |
\*-----------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>

#include "LianLiUniHubALController.h"
#include "RGBController.h"

class RGBController_LianLiUniHubAL : public RGBController
{
public:
    RGBController_LianLiUniHubAL(LianLiUniHubALController* controller_ptr);
    ~RGBController_LianLiUniHubAL();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    void SetCustomMode();

private:
    LianLiUniHubALController* controller;
    bool                    initializedMode;
};

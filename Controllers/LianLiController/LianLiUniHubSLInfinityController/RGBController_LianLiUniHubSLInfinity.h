/*-----------------------------------------*\
|  RGBController_LianLiUniHubSLInfinity.h   |
|                                           |
|  Generic RGB Interface for Lian Li Uni    |
|  Hub SL Infinity USB controller driver    |
|                                           |
|  Simon McKenna 2023-20-21                 |
|  Will Kennedy 01/17/2023                  |
|  Oliver P 04/26/2022                      |
|  Credit to Luca Lovisa for original work. |
\*-----------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>

#include "LianLiUniHubSLInfinityController.h"
#include "RGBController.h"

class RGBController_LianLiUniHubSLInfinity : public RGBController
{
public:
    RGBController_LianLiUniHubSLInfinity(LianLiUniHubSLInfinityController* controller_ptr);
    ~RGBController_LianLiUniHubSLInfinity();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    void SetCustomMode();

private:
    LianLiUniHubSLInfinityController* controller;
    bool                    initializedMode;
};

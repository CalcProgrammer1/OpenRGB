/*-----------------------------------------*\
|  RGBController_LianLiUniHubSLV2.h         |
|                                           |
|  Generic RGB Interface for Lian Li Uni    |
|  Hub SLV2 USB controller driver           |
|                                           |
|  Will Kennedy 01/17/2023					|
|  Oliver P 04/26/2022                      |
|  Credit to Luca Lovisa for original work. |
\*-----------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>

#include "LianLiUniHubSLV2Controller.h"
#include "RGBController.h"

class RGBController_LianLiUniHubSLV2 : public RGBController
{
public:
    RGBController_LianLiUniHubSLV2(LianLiUniHubSLV2Controller* controller_ptr);
    ~RGBController_LianLiUniHubSLV2();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    void SetCustomMode();

private:
    LianLiUniHubSLV2Controller* controller;
    bool                    initializedMode;
};

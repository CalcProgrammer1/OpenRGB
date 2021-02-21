/*-----------------------------------------*\
|  RGBController_LianLiUniHub.h             |
|                                           |
|  Generic RGB Interface for Lian Li Uni    |
|  Hub USB controller driver                |
|                                           |
|  Luca Lovisa 2/20/2021                    |
\*-----------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>

#include "LianLiUniHubController.h"
#include "RGBController.h"

class RGBController_LianLiUniHub : public RGBController
{
public:
    RGBController_LianLiUniHub(LianLiUniHubController* uniHub_ptr);

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    void SetCustomMode();

private:
    uint8_t convertAnyFanCount(uint8_t count);
    uint8_t convertLedSpeed(uint8_t speed);
    uint8_t convertLedDirection(uint8_t direction);

    uint8_t convertLedCountToFanCount(uint8_t count);

private:
    LianLiUniHubController* uniHub;
    bool                    initializedMode;
};

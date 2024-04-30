#pragma once

#include "RGBController.h"
#include "RoccatEloController.h"

class RGBController_RoccatElo : public RGBController
{
public:
    RGBController_RoccatElo(RoccatEloController* controller_ptr);
    ~RGBController_RoccatElo();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    RoccatEloController* controller;
};

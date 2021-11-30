/*-----------------------------------------*\
|  RGBController_RoccatVulcanAimo.h         |
|                                           |
|  Generic RGB Interface for Roccat Vulcan  |
|  Aimo controller                          |
|                                           |
|  Mola19 17/12/2021                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RoccatVulcanAimoController.h"

class RGBController_RoccatVulcanAimo : public RGBController
{
public:
    RGBController_RoccatVulcanAimo(RoccatVulcanAimoController* controller_ptr);
    ~RGBController_RoccatVulcanAimo();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    RoccatVulcanAimoController* controller;
};

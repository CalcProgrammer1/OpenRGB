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

enum
{
    ROCCAT_VULCAN_SPEED_MIN             = 0x01,
    ROCCAT_VULCAN_SPEED_MAX             = 0x0B,
    ROCCAT_VULCAN_SPEED_DEFAULT         = 0x06,
    ROCCAT_VULCAN_BRIGHTNESS_MIN        = 0x01,
    ROCCAT_VULCAN_BRIGHTNESS_MAX        = 0x45,
    ROCCAT_VULCAN_BRIGHTNESS_DEFAULT    = 0x45,
};

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

    void        DeviceUpdateMode();

private:
    RoccatVulcanAimoController* controller;
};

/*-----------------------------------------*\
|  RGBController_LogitechLightspeed.h       |
|                                           |
|  Generic RGB Interface for                |
|  Logitech Lightspeed Gaming Mice          |
|                                           |
|  TheRogueZeta   8/5/2020                  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechLightspeedController.h"

class RGBController_LogitechLightspeed : public RGBController
{
public:
    RGBController_LogitechLightspeed(LogitechLightspeedController* logitech_ptr);
    ~RGBController_LogitechLightspeed();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechLightspeedController*   logitech;
};

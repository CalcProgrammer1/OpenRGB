/*-----------------------------------------*\
|  RGBController_LogitechGLightsync1zone.h  |
|                                           |
|  Generic RGB Interface for single zone    |
|  Logitech G Lightsync Devices             |
|                                           |
|  TheRogueZeta   04/21/2021                |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechGLightsyncController.h"

class RGBController_LogitechGLightsync1zone : public RGBController
{
public:
    RGBController_LogitechGLightsync1zone(LogitechGLightsyncController* logitech_ptr);
    ~RGBController_LogitechGLightsync1zone();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechGLightsyncController*   logitech;
};

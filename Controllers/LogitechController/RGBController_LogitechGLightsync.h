/*-----------------------------------------*\
|  RGBController_LogitechGLightsync.h       |
|                                           |
|  Generic RGB Interface for                |
|  Logitech G Lightsync Devices             |
|                                           |
|  TheRogueZeta   04/21/2021                |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechGLightsyncController.h"

class RGBController_LogitechGLightsync : public RGBController
{
public:
    RGBController_LogitechGLightsync(LogitechGLightsyncController* logitech_ptr);
    ~RGBController_LogitechGLightsync();

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

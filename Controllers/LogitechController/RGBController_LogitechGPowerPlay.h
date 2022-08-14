/*-----------------------------------------*\
|  RGBController_LogitechGPowerPlay.h       |
|                                           |
|  Generic RGB Interface for                |
|  Logitech G PowerPlay Wireless Mousemat   |
|                                           |
|  TheRogueZeta   8/31/2020                 |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechGLightsyncController.h"

class RGBController_LogitechGPowerPlay : public RGBController
{
public:
    RGBController_LogitechGPowerPlay(LogitechGLightsyncController* controller_ptr);
    ~RGBController_LogitechGPowerPlay();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechGLightsyncController* controller;
};

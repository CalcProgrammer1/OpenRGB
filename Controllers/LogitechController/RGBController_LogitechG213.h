/*-----------------------------------------*\
|  RGBController_LogitechG213.h             |
|                                           |
|  Generic RGB Interface for Logitech G213  |
|                                           |
|  Eric Samuelson (edbgon) 10/06/2020       |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechG213Controller.h"

class RGBController_LogitechG213 : public RGBController
{
public:
    RGBController_LogitechG213(LogitechG213Controller* controller_ptr);
    ~RGBController_LogitechG213();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG213Controller* controller;
};

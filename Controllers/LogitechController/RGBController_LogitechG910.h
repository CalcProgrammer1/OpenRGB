/*-----------------------------------------*\
|  RGBController_LogitechG910.h             |
|                                           |
|  Generic RGB Interface for Logitech G910  |
|  Orion Spectrum keyboard                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/12/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechG910Controller.h"

class RGBController_LogitechG910 : public RGBController
{
public:
    RGBController_LogitechG910(LogitechG910Controller* logitech_ptr);
    ~RGBController_LogitechG910();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechG910Controller*   logitech;
};

/*-----------------------------------------*\
|  RGBController_LogitechG203.h             |
|                                           |
|  Generic RGB Interface for Logitech G203  |
|  Prodigy RGB Mouse                        |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/17/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechG203Controller.h"

class RGBController_LogitechG203 : public RGBController
{
public:
    RGBController_LogitechG203(LogitechG203Controller* logitech_ptr);
    ~RGBController_LogitechG203();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechG203Controller*   logitech;
};

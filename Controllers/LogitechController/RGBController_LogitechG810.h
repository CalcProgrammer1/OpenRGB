/*-----------------------------------------*\
|  RGBController_LogitechG810.h             |
|                                           |
|  Generic RGB Interface for Logitech G810  |
|  Orion Spectrum keyboard                  |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/12/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechG810Controller.h"

class RGBController_LogitechG810 : public RGBController
{
public:
    RGBController_LogitechG810(LogitechG810Controller* controller_ptr);
    ~RGBController_LogitechG810();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG810Controller* controller;
};

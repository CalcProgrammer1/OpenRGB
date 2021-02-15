/*-----------------------------------------*\
|  RGBController_LogitechG303.h             |
|                                           |
|  Generic RGB Interface for Logitech G303  |
|  Daedalus Apex Mouse                      |
|                                           |
|  TheRogueZeta   02/14/2021                |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechG303Controller.h"

class RGBController_LogitechG303 : public RGBController
{
public:
    RGBController_LogitechG303(LogitechG303Controller* logitech_ptr);
    ~RGBController_LogitechG303();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechG303Controller*   logitech;
};

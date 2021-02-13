/*-----------------------------------------*\
|  RGBController_LogitechG560.h             |
|                                           |
|  Generic RGB Interface for                |
|  G560 RGB Speaker                         |
|                                           |
|  Cheerpipe     10/28/2020                 |
|  Based on                                 |
|  TheRogueZeta   8/31/2020                 |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechG560Controller.h"

class RGBController_LogitechG560 : public RGBController
{
public:
    RGBController_LogitechG560(LogitechG560Controller* logitech_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechG560Controller*   logitech;
};

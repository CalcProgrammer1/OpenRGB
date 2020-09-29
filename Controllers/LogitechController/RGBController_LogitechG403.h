/*-----------------------------------------*\
|  RGBController_LogitechG403.h             |
|                                           |
|  Generic RGB Interface for Logitech G403  |
|  Prodigy RGB Mouse                        |
|                                           |
|  Martin Hartl (inlar) 5/19/2020           |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechG403Controller.h"

class RGBController_LogitechG403 : public RGBController
{
public:
    RGBController_LogitechG403(LogitechG403Controller* logitech_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechG403Controller*   logitech;
};

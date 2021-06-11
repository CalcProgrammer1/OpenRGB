/*-----------------------------------------*\
|  RGBController_LogitechX56.h              |
|                                           |
|  Generic RGB Interface Logitech X56       |
|  Class                                    |
|                                           |
|  Edbgon 11/06/2021                        |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechX56Controller.h"

class RGBController_LogitechX56 : public RGBController
{
public:
    RGBController_LogitechX56(LogitechX56Controller* controller_ptr);
    ~RGBController_LogitechX56();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechX56Controller*      controller;
};

/*-----------------------------------------*\
|  RGBController_LogitechG933.h             |
|                                           |
|  Generic RGB Interface for                |
|  G933 RGB Headset                         |
|                                           |
|  Edbgon        06/21/2021                 |
|  Based on                                 |
|  TheRogueZeta   8/31/2020                 |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechG933Controller.h"

class RGBController_LogitechG933 : public RGBController
{
public:
    RGBController_LogitechG933(LogitechG933Controller* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechG933Controller* controller;
};

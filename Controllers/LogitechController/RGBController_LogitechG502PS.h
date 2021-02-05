/*-----------------------------------------*\
|  RGBController_LogitechG403.h             |
|                                           |
|  Generic RGB Interface for Logitech G502  |
|  Proteus Sprectrum Mouse                  |
|                                           |
|  kernzerfall 07/28/2020                   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechG502PSController.h"

class RGBController_LogitechG502PS : public RGBController
{
public:
    RGBController_LogitechG502PS(LogitechG502PSController* logitech_ptr);
    ~RGBController_LogitechG502PS();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechG502PSController*   logitech;
};

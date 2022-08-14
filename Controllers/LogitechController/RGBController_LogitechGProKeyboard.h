/*-----------------------------------------*\
|  RGBController_LogitechGPro.h             |
|                                           |
|  Generic RGB Interface for Logitech G Pro |
|  keyboard                                 |
|                                           |
|  sanchezzzs                   20/10/2021  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechGProKeyboardController.h"

class RGBController_LogitechGProKeyboard : public RGBController
{
public:
    RGBController_LogitechGProKeyboard(LogitechGProKeyboardController* controller_ptr);
    ~RGBController_LogitechGProKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    LogitechGProKeyboardController* controller;
};

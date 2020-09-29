/*-----------------------------------------*\
|  RGBController_LogitechGProWireless.h     |
|                                           |
|  Generic RGB Interface for                |
|  Logitech G Pro Wireless Gaming Mouse     |
|                                           |
|  TheRogueZeta   8/5/2020                  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LogitechGProWirelessController.h"

class RGBController_LogitechGProWireless : public RGBController
{
public:
    RGBController_LogitechGProWireless(LogitechGProWirelessController* logitech_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LogitechGProWirelessController*   logitech;
};

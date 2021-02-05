/*-----------------------------------------*\
|  RGBController_LinuxLED.h                 |
|                                           |
|  Generic RGB Interface for Linux LED      |
|                                           |
|  Adam Honse (CalcProgrammer1) 9/25/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "LinuxLEDController.h"

class RGBController_LinuxLED : public RGBController
{
public:
    RGBController_LinuxLED(LinuxLEDController* controller_ptr);
    ~RGBController_LinuxLED();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    LinuxLEDController* controller;
};

/*-----------------------------------------*\
|  RGBController_MSI3Zone.h                 |
|                                           |
|  Generic RGB Interface for MSI/Steelseries|
|  3-Zone Keyboard                          |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "MSI3ZoneController.h"

class RGBController_MSI3Zone : public RGBController
{
public:
    RGBController_MSI3Zone(MSI3ZoneController* controller_ptr);
    ~RGBController_MSI3Zone();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    MSI3ZoneController* controller;
};

/*-----------------------------------------*\
|  RGBController_Seagate.h                  |
|                                           |
|  Generic RGB Interface for Seagate        |
|                                           |
|  Adam Honse (CalcProgrammer1) 11/8/2022   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "SeagateController.h"

class RGBController_Seagate : public RGBController
{
public:
    RGBController_Seagate(SeagateController* controller_ptr);
    ~RGBController_Seagate();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        DeviceSaveMode();

private:
    SeagateController* controller;
};

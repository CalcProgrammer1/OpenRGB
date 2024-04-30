/*-----------------------------------------*\
|  RGBController_AsusROGAlly.cpp            |
|                                           |
|  Generic RGB Interface for Asus ROG Ally  |
|  controller driver                        |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/12/2023   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AsusROGAllyController.h"

class RGBController_AsusROGAlly : public RGBController
{
public:
    RGBController_AsusROGAlly(ROGAllyController* controller_ptr);
    ~RGBController_AsusROGAlly();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    ROGAllyController* controller;
};

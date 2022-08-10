/*-----------------------------------------*\
|  RGBController_AsusStrixClaw.h            |
|                                           |
|  Generic RGB Interface for Asus           |
|  Legacy USB controller driver             |
|                                           |
|  Mola19 08/06/2022                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusStrixClawController.h"


class RGBController_StrixClaw : public RGBController
{
public:
    RGBController_StrixClaw(StrixClawController* controller_ptr);
    ~RGBController_StrixClaw();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    StrixClawController*  controller;
};

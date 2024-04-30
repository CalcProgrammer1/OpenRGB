/*-------------------------------------------------------------------*\
|  RGBController_AsusROGStrixLC.h                                     |
|                                                                     |
|  Driver for the ASUS Aura Liquid Cooler (LC) series                 |
|    of lighting controllers                                          |
|                                                                     |
|  Chris M (Dr_No)          17 Jan 2022                               |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "LogManager.h"
#include "RGBController.h"
#include "AsusROGStrixLCController.h"
#include <vector>

class RGBController_AsusROGStrixLC : public RGBController
{
public:
    RGBController_AsusROGStrixLC(AsusROGStrixLCController* controller_ptr);
    ~RGBController_AsusROGStrixLC();

    void                SetupZones();
    void                ResizeZone(int zone, int new_size);

    void                DeviceUpdateLEDs();
    void                UpdateZoneLEDs(int zone);
    void                UpdateSingleLED(int led);

    void                DeviceUpdateMode();
private:
    int                 GetDeviceMode();
    int                 GetLED_Zone(int led_idx);

    AsusROGStrixLCController* controller;
};

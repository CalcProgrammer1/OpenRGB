/*-----------------------------------------*\
|  RGBController_AsusAuraMonitor.h          |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Mola19 08/03/2022                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusAuraMonitorController.h"

class RGBController_AuraMonitor : public RGBController
{
public:
    RGBController_AuraMonitor(AuraMonitorController* controller_ptr);
    ~RGBController_AuraMonitor();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AuraMonitorController*  controller;
};

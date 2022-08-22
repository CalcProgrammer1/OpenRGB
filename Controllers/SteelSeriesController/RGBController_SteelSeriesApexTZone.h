/*-----------------------------------------*\
|  RGBController_SteelSeriesApexTZone.h     |
|                                           |
|  Edbgon 06.10.21                          |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesApexTZoneController.h"

class RGBController_SteelSeriesApexTZone : public RGBController
{
public:
    RGBController_SteelSeriesApexTZone(SteelSeriesApexTZoneController* apex_tzone_ptr);
    ~RGBController_SteelSeriesApexTZone();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    SteelSeriesApexTZoneController*     controller;
};

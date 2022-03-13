/*---------------------------------------------------------*\
|  RGBController_AcerMonitor.h                              |
|                                                           |
|  RGBController interface implementation for 2020/2021     |
|  Acer monitors with rear RGB strips.                      |
|                                                           |
|  Shenghao Yang (me@shenghaoyang.info), 30/12/2021         |
|  Amended for code style by Adam Honse, 3/13/2022          |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AcerMonitorController.h"

class RGBController_AcerMonitor : public RGBController
{
public:
    RGBController_AcerMonitor(AcerMonitorController* controller_ptr);
    ~RGBController_AcerMonitor();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    AcerMonitorController* controller;
};

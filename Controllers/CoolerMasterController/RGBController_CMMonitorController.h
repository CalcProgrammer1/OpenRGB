/*-------------------------------------------------------------------*\
|  RGBController_CMMonitorController.cpp                              |
|                                                                     |
|  Driver for Coolermaster Gaming Monitor USB Controller              |
|                                                                     |
|  morg (Morgan Guimard) 9/18/2023                                    |
|                                                                     |
\*-------------------------------------------------------------------*/
#pragma once
#include "RGBController.h"
#include "CMMonitorController.h"
#include <vector>

class RGBController_CMMonitorController : public RGBController
{
public:
    RGBController_CMMonitorController(CMMonitorController* controller_ptr);
    ~RGBController_CMMonitorController();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);
    void SetCustomMode();
    void DeviceUpdateMode();

private:   
    CMMonitorController*   controller;
};

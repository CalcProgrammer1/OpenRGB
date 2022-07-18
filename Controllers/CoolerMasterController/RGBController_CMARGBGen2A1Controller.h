/*-------------------------------------------------------------------*\
|  RGBController_CMARGBGen2A1Controller.h                             |
|                                                                     |
|  Driver for Coolermaster ARGB Controller                            |
|                                                                     |
|  morg (Morgan Guimard) 6/26/2022                                    |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CMARGBGen2A1controller.h"
#include <vector>

class RGBController_CMARGBGen2A1Controller : public RGBController
{
public:
    RGBController_CMARGBGen2A1Controller(CMARGBGen2A1controller* controller_ptr);
    ~RGBController_CMARGBGen2A1Controller();

    void SetupZones();
    void ResizeZone(int zone, int new_size);
    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);
    void SetCustomMode();
    void DeviceUpdateMode();
    void DeviceSaveMode();

private:   
    CMARGBGen2A1controller*   controller;
};

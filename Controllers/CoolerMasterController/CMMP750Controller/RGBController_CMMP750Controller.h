/*-------------------------------------------------------------------*\
|  RGBController_CMMP750Controller.h                                  |
|                                                                     |
|  Driver for Coolermaster MP750 mousepad                             |
|                                                                     |
|  Chris M (Dr_No)          18th Apr 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CMMP750Controller.h"

class RGBController_CMMP750Controller : public RGBController
{
public:
    RGBController_CMMP750Controller(CMMP750Controller* controller_ptr);
    ~RGBController_CMMP750Controller();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
private:
    CMMP750Controller*  controller;

    int         GetDeviceMode();
};

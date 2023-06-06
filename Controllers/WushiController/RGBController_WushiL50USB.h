/*-------------------------------------*\
|  RGBController_WushiL50USB.h          |
|                                       |
|  interface for Wushi L50 Devices      |
\*-------------------------------------*/

#pragma once

#include "WushiL50USBController.h"
#include "RGBController.h"

#include <vector>

class RGBController_WushiL50USB : public RGBController
{
public:
    RGBController_WushiL50USB(WushiL50USBController* controller_ptr);
    ~RGBController_WushiL50USB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    WushiL50USBController * controller;
    WushiL50State           state;
};

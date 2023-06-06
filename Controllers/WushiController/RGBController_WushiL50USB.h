/*-------------------------------------------------------------------*\
|  RGBController_WushiL50USB.h                                     |
|                                                                     |
|  interface for Wushi L50 Devices                               |
\*-------------------------------------------------------------------*/

#pragma once

#include "WushiDevices.h"
#include "WushiL50USBController.h"
#include "RGBController.h"

#include <vector>

#define NA 0xFFFFFFFF

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
    KeyboardState state;

    WushiL50USBController *controller;
};

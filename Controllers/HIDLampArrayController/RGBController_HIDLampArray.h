/*-----------------------------------------------------*\
|  RGBController_HIDLampArray.h                         |
|                                                       |
|  Generic RGB Interface HID LampArray Devices          |
|                                                       |
|  Adam Honse <calcprogrammer1@gmail.com>   3/26/2024   |
\*-----------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "HIDLampArrayController.h"

class RGBController_HIDLampArray : public RGBController
{
public:
    RGBController_HIDLampArray(HIDLampArrayController* controller_ptr);
    ~RGBController_HIDLampArray();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    HIDLampArrayController* controller;
};

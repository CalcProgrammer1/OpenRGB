/*------------------------------------------------*\
|  RGBController_JginYueInternalUSB.h              |
|                                                  |
|  Generic RGB Interface JginYueInternalUSB Class  |
|                                                  |
|  Tong R    (tcr020)  2023/08/09                  |
|  Liu  ShiMeng    (Moon dream stars)  2003/08/09  |
|                                                  |
|  Dongguan Yonghang Electronic Technology Co., Ltd|
\*------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "JginYueInternalUSBController.h"

class RGBController_JginYueInternalUSB : public RGBController
{
public:
    RGBController_JginYueInternalUSB(JginYueInternalUSBController* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    JginYueInternalUSBController*        controller;
    bool                        initializedMode;
};

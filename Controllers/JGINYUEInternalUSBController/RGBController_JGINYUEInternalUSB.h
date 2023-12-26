/*------------------------------------------------*\
|  RGBController_JGINYUEInternalUSB.h              |
|                                                  |
|  Generic RGB Interface JGINYUEInternalUSB Class  |
|                                                  |
|  Tong R    (tcr020)  2023/08/09                  |
|  Liu  ShiMeng    (Moon dream stars)  2003/08/09  |
|                                                  |
|  Dongguan Yonghang Electronic Technology Co., Ltd|
\*------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "JGINYUEInternalUSBController.h"

class RGBController_JGINYUEInternalUSB : public RGBController
{
public:
    RGBController_JGINYUEInternalUSB(JGINYUEInternalUSBController* controller_ptr);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    JGINYUEInternalUSBController*   controller;
};

/*-----------------------------------------*\
|  RGBController_KasaSmart.h                |
|                                           |
|  Generic RGB Interface for Kasa Smart     |
|                                           |
|  Devin Wendt (umbreon222) 2/16/2023       |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "KasaSmartController.h"

class RGBController_KasaSmart : public RGBController
{
public:
    RGBController_KasaSmart(KasaSmartController* controller_ptr);
    ~RGBController_KasaSmart();

    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    KasaSmartController* controller;
};

/*-----------------------------------------*\
|  RGBController_ZotacV2GPU.h               |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ZOTAC RTX 30/40 series GPU Driver        |
|                                           |
|  Krzysztof Haładyn (krzys_h)  3/16/2023   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ZotacV2GPUController.h"

class RGBController_ZotacV2GPU : public RGBController
{
public:
    RGBController_ZotacV2GPU(ZotacV2GPUController* controller_ptr);
    ~RGBController_ZotacV2GPU();

    void SetupInitialValues();
    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

    ZotacV2GPUConfig config;

private:
    ZotacV2GPUController* controller;

    int FindSynchronizedZoneNum(int syncMode);
};

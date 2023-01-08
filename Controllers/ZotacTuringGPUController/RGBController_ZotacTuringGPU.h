/*-----------------------------------------*\
|  RGBController_ZotacTuringGPU.h           |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  ZOTAC Turing GPU Driver                  |
|                                           |
|  David Henry                  1/07/2023   |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "ZotacTuringGPUController.h"

class RGBController_ZotacTuringGPU : public RGBController
{
public:
    RGBController_ZotacTuringGPU(ZotacTuringGPUController* controller_ptr);
    ~RGBController_ZotacTuringGPU();

    void SetupInitialValues();
    void SetupZones();

    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    ZotacTuringGPUController* controller;
};

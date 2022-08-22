/*-------------------------------------------*\
|  RGBController_Sinowealth1007.h             |
|                                             |
|  Generic RGB Interface for Sinowealth       |
|  mice with PID 1007 and similar controllers |
|                                             |
|  Moon_darker (Vaker) 25.01.2022             |
\*-------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SinowealthController1007.h"

class RGBController_Sinowealth1007 : public RGBController
{
public:
    RGBController_Sinowealth1007(SinowealthController1007* controller_ptr);
    ~RGBController_Sinowealth1007();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();


private:
    SinowealthController1007* controller;
};

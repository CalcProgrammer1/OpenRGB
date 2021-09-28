/*-----------------------------------------*\
|  RGBController_AnnePro2.h                 |
|                                           |
|  Generic RGB Interface for                |
|  AnnePro2 w/QMK keyboard                  |
|                                           |
|  Sergey Gavrilov (DrZlo13) 06/06/2021     |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AP2Controller.h"

class RGBController_AP2 : public RGBController
{
public:
    RGBController_AP2(AP2Controller* annepro2_ptr);
    ~RGBController_AP2();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    AP2Controller*   annepro2;
};

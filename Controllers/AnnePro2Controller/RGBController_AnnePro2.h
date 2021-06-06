/*-----------------------------------------*\
|  RGBController_AnnePro2.h                 |
|                                           |
|  Generic RGB Interface for Obins Lab      |
|  AnnePro2 keyboard                        |
|                                           |
|  Sergey Gavrilov (DrZlo13) 06/06/2021     |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AnnePro2Controller.h"

class RGBController_AnnePro2 : public RGBController
{
public:
    RGBController_AnnePro2(AnnePro2Controller* annepro2_ptr);
    ~RGBController_AnnePro2();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    AnnePro2Controller*   annepro2;
};
